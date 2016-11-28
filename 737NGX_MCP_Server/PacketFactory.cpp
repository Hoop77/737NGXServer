#include "PacketFactory.h"
#include "ProtocolException.h"
#include "TCPReader.h"
#include "TCPException.h"


using namespace Protocol;


std::unique_ptr<Packet>
PacketFactory::readPacketFromStream( TCP::Stream *stream )
{
	ReadArgs args( stream );
	
	try
	{
		readPacket( args );
	}
	catch( TCP::Exception & e )
	{
		throw Exception( "TCP error: " + e.what() );
	}

	return std::move( args.packetPtr );
}


void
PacketFactory::readNext( ReadArgs & args, size_t len )
{
	// read packet type and entity-IDbytesToRead = 2;
	bool success = args.reader.read( args.buf, PACKET_MAX_SIZE, args.pos, len );
	if( !success )
		throw Exception( "could not read packet from client" );
	args.pos += len;
}


void 
PacketFactory::readPacket( ReadArgs & args )
{
	// read packet type and entity-ID
	readNext( args, 2 );
	uint8_t packetType = args.buf[ 0 ];
	uint8_t entityId = args.buf[ 1 ];

	switch( packetType )
	{
		// EventPacket
		case PacketType::EVENT:
			readEventPacket( args, entityId );
			break;

		// RequestPacket
		case PacketType::REQUEST:
			readRequestPacket( args, entityId );
			break;

		default:
			throw Exception( "invalid packet type" );
			break;
	}
}


void 
PacketFactory::readEventPacket( ReadArgs & args, uint32_t entityId )
{
	// read event-ID and parameter
	readNext( args, 8 );

	uint32_t eventId = 
		args.buf[ 2 ]
		+ (args.buf[ 3 ] << 8)
		+ (args.buf[ 4 ] << 16)
		+ (args.buf[ 5 ] << 24);

	uint32_t eventParameter =
		args.buf[ 6 ]
		+ (args.buf[ 7 ] << 8 )
		+ (args.buf[ 8 ] << 16)
		+ (args.buf[ 9 ] << 24);

	// create event packet
	args.packetPtr = createEventPacket( entityId, eventId, eventParameter );
}


void 
PacketFactory::readRequestPacket( ReadArgs & args, uint32_t entityId )
{
	// read request type
	readNext( args, 1 );
	uint8_t requestType = args.buf[ 2 ];

	switch( requestType )
	{
		// SingleValueRequestPacket
		case RequestType::SINGLE_VALUE:
			readSingleValueRequestPacket( args, entityId );
			break;

		// AllValuesRequestPacket
		case RequestType::ALL_VALUES:
			readAllValuesRequestPacket( args, entityId );
			break;

		default:
			throw Exception( "invalid request type" );
			break;
	}
}


void 
PacketFactory::readSingleValueRequestPacket( ReadArgs & args, uint32_t entityId )
{
	// read value-ID
	readNext( args, 2 );
	uint16_t valueId = args.buf[ 3 ] + (args.buf[ 4 ] << 8);

	// create single-value-request packet
	args.packetPtr = createSingleValueRequestPacket( entityId, valueId );
}


void 
PacketFactory::readAllValuesRequestPacket( ReadArgs & args, uint32_t entityId )
{
	// nothing to read
	// create all-values-request packet
	args.packetPtr = createAllValuesRequestPacket( entityId );
}


void
PacketFactory::writePacketToStream( Packet *packet, TCP::Stream *stream )
{
	uint8_t raw[ PACKET_MAX_SIZE ];
	size_t len = 0;

	switch( packet->packetType )
	{
		//
		// EventPacket
		//
		case PacketType::EVENT:
			// not needed
			break;

		//
		// RequestPacket
		//
		case PacketType::REQUEST:
			// not needed
			break;

		//
		// DataPacket
		//
		case PacketType::DATA:
		{
			DataPacket *dataPacket = dynamic_cast<DataPacket *>(packet);
			switch( dataPacket->requestType )
			{
				//
				// SingleValueDataPacket
				//
				case RequestType::SINGLE_VALUE:
				{
					SingleValueDataPacket *singleValueDataPacket = dynamic_cast<SingleValueDataPacket *>(packet);

					len = getRawSingleValueDataPacket( singleValueDataPacket, raw );

					break;
				}

				//
				// AllValuesDataPacket
				//
				case RequestType::ALL_VALUES:
				{
					AllValuesDataPacket *allValuesDataPacket = dynamic_cast<AllValuesDataPacket *>(packet);

					len = getRawAllValuesDataPacket( allValuesDataPacket, raw );

					break;
				}

				default:
					throw Exception( "invalid request type" );
					break;
			}

			break;
		}

		default:
			throw Exception( "invalid packet type" );
			break;
	}

	// send raw data to stream
	if( len > 0 )
	{
		try
		{
			stream->write( (char *) raw, len );
		}
		catch( TCP::Exception & e )
		{
			throw Exception( "TCP error: " + e.what() );
		}
	}
}


size_t
PacketFactory::getRawSingleValueDataPacket( SingleValueDataPacket *singleValueDataPacket, uint8_t *raw )
{
	raw[ 0 ] = singleValueDataPacket->packetType;
	raw[ 1 ] = singleValueDataPacket->entityId;
	raw[ 2 ] = singleValueDataPacket->requestType;
	std::memcpy( raw + 3, &singleValueDataPacket->valueId, 2 );
	std::memcpy( raw + 5, &singleValueDataPacket->value, 4 );

	return 9;
}


size_t
PacketFactory::getRawAllValuesDataPacket( AllValuesDataPacket *allValuesDataPacket, uint8_t *raw )
{
	raw[ 0 ] = allValuesDataPacket->packetType;
	raw[ 1 ] = allValuesDataPacket->entityId;
	raw[ 2 ] = allValuesDataPacket->requestType;
	std::memcpy( raw + 3, allValuesDataPacket->values.get(), allValuesDataPacket->valueCount * 4 );

	return 3 + allValuesDataPacket->valueCount * 4;
}


std::unique_ptr<Packet> 
PacketFactory::createEventPacket( uint32_t entityId, uint32_t eventId, uint32_t eventParameter )
{
	std::unique_ptr<Packet> packet( new EventPacket() );

	EventPacket *eventPacket = dynamic_cast<EventPacket *>( packet.get() );
	eventPacket->packetType = PacketType::EVENT;
	eventPacket->entityId = entityId;
	eventPacket->eventId = eventId;
	eventPacket->eventParameter = eventParameter;

	return packet;
}


std::unique_ptr<Packet> 
PacketFactory::createSingleValueRequestPacket( uint32_t entityId, uint16_t valueId )
{
	std::unique_ptr<Packet> packet( new SingleValueRequestPacket() );

	SingleValueRequestPacket *singleValueRequestPacket = dynamic_cast<SingleValueRequestPacket *>( packet.get() );
	singleValueRequestPacket->packetType = PacketType::REQUEST;
	singleValueRequestPacket->entityId = entityId;
	singleValueRequestPacket->requestType = RequestType::SINGLE_VALUE;
	singleValueRequestPacket->valueId = valueId;

	return packet;
}


std::unique_ptr<Packet> 
PacketFactory::createAllValuesRequestPacket( uint32_t entityId )
{
	std::unique_ptr<Packet> packet( new AllValuesRequestPacket() );

	AllValuesRequestPacket *allValuesRequestPacket = dynamic_cast<AllValuesRequestPacket *>( packet.get() );
	allValuesRequestPacket->packetType = PacketType::REQUEST;
	allValuesRequestPacket->entityId = entityId;
	allValuesRequestPacket->requestType = RequestType::ALL_VALUES;

	return packet;
}


std::unique_ptr<Packet>
PacketFactory::createSingleValueDataPacket( uint8_t entityId, uint16_t valueId, uint32_t value )
{
	std::unique_ptr<Packet> packet( new SingleValueDataPacket() );

	SingleValueDataPacket *singleValueDataPacket = dynamic_cast<SingleValueDataPacket *>(packet.get());
	singleValueDataPacket->packetType = PacketType::DATA;
	singleValueDataPacket->entityId = entityId;
	singleValueDataPacket->requestType = RequestType::SINGLE_VALUE;
	singleValueDataPacket->valueId = valueId;
	singleValueDataPacket->value = value;

	return packet;
}


std::unique_ptr<Packet>
PacketFactory::createAllValuesDataPacket( uint8_t entityId, std::unique_ptr<uint32_t> values, size_t valueCount )
{
	std::unique_ptr<Packet> packet( new AllValuesDataPacket() );

	AllValuesDataPacket *allValuesDataPacket = dynamic_cast<AllValuesDataPacket *>(packet.get());
	allValuesDataPacket->packetType = PacketType::DATA;
	allValuesDataPacket->entityId = entityId;
	allValuesDataPacket->requestType = RequestType::ALL_VALUES;
	allValuesDataPacket->values = std::move( values );
	allValuesDataPacket->valueCount = valueCount;

	return packet;
}
