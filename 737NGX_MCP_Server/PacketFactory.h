#pragma once


#include "Packet.h"
#include "TCPStream.h"
#include "TCPReader.h"

#include <memory>


namespace Protocol
{
	class Packet;
	class DataPacket;

	class PacketFactory
	{
	public:
		static std::unique_ptr<Packet> readPacketFromStream( TCP::Stream *stream );
		static void writePacketToStream( Packet *packet, TCP::Stream *stream );

		static std::unique_ptr<DataPacket> createSingleValueDataPacket( uint8_t entityId, uint16_t valueId, uint32_t value );
		static std::unique_ptr<DataPacket> createAllValuesDataPacket( uint8_t entityId, uint32_t *values, size_t valueCount );

	private:
		static void readNext( TCP::Reader & reader, uint8_t *buf, size_t *pos, size_t len );
	};
}