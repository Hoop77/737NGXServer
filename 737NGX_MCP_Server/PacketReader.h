#pragma once


#include "TCPStream.h"
#include "TCPReader.h"
#include "Packet.h"


#include <memory>


namespace Protocol
{
	class PacketReader
	{
	public:
		

	private:
		std::unique_ptr<Packet> packetPtr;
		TCP::Reader reader;
		uint8_t *buf;
		size_t pos;
	};
}
