#include "PacketFactory.h"
#include "ProtocolException.h"
#include "TCPReader.h"
#include "TCPException.h"


using namespace Protocol;


std::unique_ptr<Packet>
PacketFactory::readPacketFromStream( TCP::Stream *stream )
{
	std::unique_ptr<Packet> packet( nullptr );
	TCP::Reader reader( stream, PACKET_MAX_SIZE );
	uint8_t buf[ PACKET_MAX_SIZE ];
	size_t pos = 0;

	try
	{
		// read packet type and entity-ID
		readNext( reader, buf, &pos, 2 );
		uint8_t packetType = buf[ 0 ];
		uint8_t entityId = buf[ 1 ];

		switch( packetType )
		{
		case PacketType::EVENT:
			packet.reset( new EventPacket() );
			break;

		case PacketType::REQUEST:
			// read request type
			readNext( reader, buf, &pos, 1 );
			uint8_t requestType = buf[ 2 ];
			
			switch( requestType )
			{
			case RequestType::SINGLE_VALUE:
				// read value-ID
				readNext( reader, buf, &pos, 2 );
				uint16_t valueId = buf[ 3 ] + (buf[ 4 ] << 8);
				packet.reset( new SingleValueRequestPacket() );

				// assign value-ID
				auto singleValuePacket = dynamic_cast<SingleValueRequestPacket *>( packet.get() );
				singleValuePacket->valueId = valueId;

				break;

			case RequestType::ALL_VALUES:
				// no need to read anything
				packet.reset( new AllValuesRequestPacket() );
				break;

			default:
				throw Exception( "invalid request type" );
				break;
			}

			// assign request type
			auto requestPacket = dynamic_cast<RequestPacket *>( packet.get() );
			requestPacket->requestType = requestType;

			break;
		
		default:
			throw Exception( "invalid packet type" );
			break;
		}

		// assign packet type and entity-ID
		packet->entityId = entityId;
		packet->packetType = packetType;
	}
	catch( TCP::Exception & e )
	{
		throw Exception( std::string( "TCP error: " ).append( e.what() ) );
	}

	return packet;
}


void 
PacketFactory::readNext( TCP::Reader & reader, uint8_t *buf, size_t *pos, size_t len )
{
	// read packet type and entity-IDbytesToRead = 2;
	bool success = reader.read( buf, PACKET_MAX_SIZE, *pos, len );
	if( !success )
		throw Exception( "could not read packet from client" );
	*pos += len;
}


void
PacketFactory::writePacketToStream( Packet *packet, TCP::Stream *stream )
{
	uint8_t raw[ PACKET_MAX_SIZE ];
	size_t len = 0;

	switch( packet->packetType )
	{
	case PacketType::EVENT:
		// not needed
		break;
	
	case PacketType::REQUEST:
		// not needed
		break;

	case PacketType::DATA:
		DataPacket *dataPacket = dynamic_cast<DataPacket *>( packet );
		switch( dataPacket->requestType )
		{
		case RequestType::SINLGE_VALUE:
			SingleValueDataPacket *singleValueDataPacket = dynamic_cast<SingleValueDataPacket *>( packet );
			
			raw[ 0 ] = singleValueDataPacket->packetType;
			raw[ 1 ] = singleValueDataPacket->entityId;
			raw[ 2 ] = singleValueDataPacket->requestType;
			std::memcpy( raw + 3, &singleValueDataPacket->valueId, 2 );
			std::memcpy( raw + 5, &singleValueDataPacket->value, 4 );

			len = 9;

			break;

		case RequestType::ALL_VALUES:
			AllValuesDataPacket *allValuesDataPacket = dynamic_cast<AllValuesDataPacket *>( packet );

			raw[ 0 ] = singleValueDataPacket->packetType;
			raw[ 1 ] = singleValueDataPacket->entityId;
			raw[ 2 ] = singleValueDataPacket->requestType;
			for( size_t i = 0; i < allValuesDataPacket->valueCount; i++ )
			{
				std::memcpy( raw + 3 + i * 4, &allValuesDataPacket->values[ i ], 4 );
			}

			len = 3 + allValuesDataPacket->valueCount * 4;

			break;

		default:
			throw Exception( "invalid request type" );		
			break;
		}

		break;

	default:
		throw Exception( "invalid packet type" );
		break;
	}

	// send raw data to stream
	if( len > 0 )
	{
		try
		{
			stream.write( (char *) raw, len );
		}
		catch( TCP::Exception & e )
		{
			throw Exception( std::string( "TCP error: " ).append( e.what() ) );
		}
	}

	return len;
}


std::unique_ptr<DataPacket> 
createSingleValueDataPacket( uint8_t entityId, uint16_t valueId, uint32_t value );
{
	std::unique_ptr<DataPacket> dataPacket( new SingleValueDataPacket() );

	SingleValueDataPacket *singleValueDataPacket = dynamic_cast<SingleValueDataPacket *>( dataPacket );
	singleValueDataPacket->packetType = PacketType::DATA;
	singleValueDataPacket->entityId = entityId;
	singleValueDataPacket->requestType = RequestType::SINGLE_VALUE;
	singleValueDataPacket->valueId = valueId;
	singleValueDataPacket->value = value;
	
	return dataPacket;
}


std::unique_ptr<DataPacket> 
createAllValuesDataPacket( uint8_t entityId, uint32_t *values, size_t valueCount )
{
	std::unique_ptr<DataPacket> dataPacket( new AllValuesDataPacket() );

	AllValuesDataPacket *allValuesDataPacket = dynamic_cast<AllValuesDataPacket *>( DataPacket );
	allValuesDataPacket->packetType = PacketType::DATA;
	allValuesDataPacket->entityId = entityId;
	allValuesDataPacket->requestType = RequestType::ALL_VALUES;
	allValuesDataPacket->values = values;
	allValuesDataPacket->valueCount = valueCount;

	return dataPacket;
}