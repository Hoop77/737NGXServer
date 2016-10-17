#pragma once


#include "packet.h"


#include <memory>


namespace Protocol
{
	class PacketFactory
	{
	public:
		static std::unique_ptr<Packet> createPacketFromReceivedData( const char *data, size_t size );
		static std::unique_ptr<DataPacket> createSingleValueDataPacket( unsigned int entityId, unsigned int valueId, uint32_t value );
		static std::unique_ptr<DataPacket> createAllValuesDataPacket( int entityId, uint32_t *values, size_t valueCount );
	};
}