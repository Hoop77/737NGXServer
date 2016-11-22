#include "PacketFactory.h"
#include "ProtocolException.h"
#include "TCPReader.h"
#include "TCPException.h"

using namespace Protocol;


std::unique_ptr<Packet>
PacketFactory::createPacketFromReceivedData( TCP::Stream *stream )
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

		if( packetType == PacketType::EVENT )
		{
			packet.reset( new EventPacket() );
		}
		else if( packetType == PacketType::REQUEST )
		{
			// read request type
			readNext( reader, buf, &pos, 1 );
			uint8_t requestType = buf[ 2 ];
			
			if( requestType == RequestType::SINGLE_VALUE )
			{
				// read value-ID
				readNext( reader, buf, &pos, 2 );
				uint16_t valueId = buf[ 3 ] + (buf[ 4 ] << 8);
				packet.reset( new SingleValueRequestPacket() );

				// assign value-ID
				auto singleValuePacket = dynamic_cast<SingleValueRequestPacket *>( packet.get() );
				singleValuePacket->valueId = valueId;
			}
			else if( requestType == RequestType::ALL_VALUES )
			{
				// no need to read anything
				packet.reset( new AllValuesRequestPacket() );
			}
			else
			{
				throw Exception( "invalid request type" );
			}

			// assign request type
			auto requestPacket = dynamic_cast<RequestPacket *>( packet.get() );
			requestPacket->requestType = requestType;
		}
		else
		{
			throw Exception( "invalid packet type" );
		}

		// assign packet type and entity-ID
		packet->entityId = entityId;
		packet->packetType = packetType;
	}
	catch( TCP::Exception )
	{
		throw Exception( "TCP error" );
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
PacketFactory::createRawPacketData( Packet *packet, uint8_t *buf, size_t *len )
{
	
}