#include "PacketIO.h" 
#include "ProtocolException.h"
#include "TCPException.h"


using namespace Protocol;


std::unique_ptr<Packet>
PacketIO::readPacketFromStream( TCP::Stream *stream )
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
PacketIO::readNext( ReadArgs & args, size_t len )
{
	bool success = args.reader.read( args.buf, PACKET_MAX_SIZE, args.pos, len );
	if( !success )
		throw Exception( "could not read packet from client" );
	args.pos += len;
}


void
PacketIO::readPacket( ReadArgs & args )
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
PacketIO::readEventPacket( ReadArgs & args, uint32_t entityId )
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
		+ (args.buf[ 7 ] << 8)
		+ (args.buf[ 8 ] << 16)
		+ (args.buf[ 9 ] << 24);

	// create event packet
	args.packetPtr.reset( new EventPacket( entityId, eventId, eventParameter ) );
}


void
PacketIO::readRequestPacket( ReadArgs & args, uint32_t entityId )
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
PacketIO::readSingleValueRequestPacket( ReadArgs & args, uint32_t entityId )
{
	// read value-ID
	readNext( args, 2 );
	uint16_t valueId = args.buf[ 3 ] + (args.buf[ 4 ] << 8);

	// create single-value-request packet
	args.packetPtr.reset( new SingleValueRequestPacket( entityId, valueId ) );
}


void
PacketIO::readAllValuesRequestPacket( ReadArgs & args, uint32_t entityId )
{
	// nothing to read
	// create all-values-request packet
	args.packetPtr.reset( new AllValuesRequestPacket( entityId ) );
}


void
PacketIO::writePacketToStream( Packet *packet, TCP::Stream *stream )
{
	uint8_t raw[ PACKET_MAX_SIZE ];
	size_t len = 0;

	switch( packet->getPacketType() )
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
			switch( dataPacket->getRequestType() )
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
PacketIO::getRawSingleValueDataPacket( SingleValueDataPacket *singleValueDataPacket, uint8_t *raw )
{
	raw[ 0 ] = singleValueDataPacket->getPacketType();
	raw[ 1 ] = singleValueDataPacket->getEntityId();
	raw[ 2 ] = singleValueDataPacket->getRequestType();
	uint16_t valueId = singleValueDataPacket->getValueId();
	raw[ 3 ] = valueId & 0x00ff;
	raw[ 4 ] = (valueId & 0xff00) >> 8;
	uint32_t value = singleValueDataPacket->getValue();
	raw[ 5 ] = value & 0x000000ff;
	raw[ 6 ] = (value & 0x0000ff00) >> 8;
	raw[ 7 ] = (value & 0x00ff0000) >> 16;
	raw[ 8 ] = (value & 0xff000000) >> 24;

	return 9;
}


size_t
PacketIO::getRawAllValuesDataPacket( AllValuesDataPacket *allValuesDataPacket, uint8_t *raw )
{
	raw[ 0 ] = allValuesDataPacket->getPacketType();
	raw[ 1 ] = allValuesDataPacket->getEntityId();
	raw[ 2 ] = allValuesDataPacket->getRequestType();
	std::memcpy( raw + 3, allValuesDataPacket->getValues(), allValuesDataPacket->getValueCount() * 4 );

	return 3 + allValuesDataPacket->getValueCount() * 4;
}