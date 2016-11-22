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
		static std::unique_ptr<Packet> createPacketFromReceivedData( TCP::Stream *stream );
		static void PacketFactory::createRawPacketData( Packet *packet, uint8_t *buf, size_t *len );

	private:
		static void PacketFactory::readNext( TCP::Reader & reader, uint8_t *buf, size_t *pos, size_t len );
	};
}