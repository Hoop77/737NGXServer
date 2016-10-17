#include "PacketFactory.h"


using namespace Protocol;


std::unique_ptr<Packet> 
PacketFactory::createPacketFromReceivedData( const char *data, size_t size )
{
	if( data == nullptr )
	{
		throw PacketException( "nullptr buffer" );
	}

	// starting with a nullptr
	std::unique_ptr<Packet> packet( nullptr );

	// checkout packet type
	int packetType = data[ 0 ];
	if( packetType == Packet::PACKET_TYPE_EVENT )
	{
		packet.reset( new EventPacket() );
	}
	else if( packetType == Packet::PACKET_TYPE_REQUEST )
	{
		// validate request type
		int requestType = data[ RequestPacket::BYTE_POS_REQUEST_TYPE ];

		if( requestType == RequestPacket::REQUEST_TYPE_SINGLE_VALUE )
		{
			packet.reset( new SingleValueRequestPacket() );
		}
		else if( requestType == RequestPacket::REQUEST_TYPE_ALL_VALUES )
		{
			packet.reset( new AllValuesRequestPacket( size ) );
		}
		else
		{
			throw PacketException( "invalid request type" );
		}
	}
	else
	{
		throw PacketException( "invalid packet type" );
	}

	// size check
	size_t packetSize = packet->getSize();
	if( size < packetSize )
	{
		throw PacketException( "insufficient buffer" );
	}

	// If everything went wrong, copy the data into the packet.
	std::memcpy( packet->getData(), data, packetSize );

	return packet;
}


std::unique_ptr<DataPacket> 
PacketFactory::createSingleValueDataPacket( unsigned int entityId, unsigned int valueId, uint32_t value )
{
	return std::unique_ptr<DataPacket>( 
		new SingleValueRequestPacket( entityId, valueId, value ) );
}


std::unique_ptr<DataPacket> 
PacketFactory::createAllValuesDataPacket( int entityId, uint32_t *values, size_t valueCount )
{
	return std::unique_ptr<DataPacket>(
		new AllValuesDataPacket( entityId, values, valueCount ) );
}