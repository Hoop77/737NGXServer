#pragma once


#include "Packet.h"
#include "TCPStream.h"
#include "TCPReader.h"

#include <memory>


namespace Protocol
{
	class PacketIO
	{
	public:
		static std::unique_ptr<Packet> readPacketFromStream( TCP::Stream *stream );
		static void writePacketToStream( Packet *packet, TCP::Stream *stream );

	private:
		struct ReadArgs
		{
			ReadArgs( TCP::Stream *stream )
				: reader( stream )
				, pos( 0 ) {}

			std::unique_ptr<Packet> packetPtr;
			TCP::Reader reader;
			uint8_t buf[ PACKET_MAX_SIZE ];
			size_t pos;
		};

		static void readNext( ReadArgs & args, size_t len );

		static void readPacket( ReadArgs & args );
		static void readEventPacket( ReadArgs & args, uint32_t entityId );
		static void readRequestPacket( ReadArgs & args, uint32_t entityId );
		static void readSingleValueRequestPacket( ReadArgs & args, uint32_t entityId );
		static void readAllValuesRequestPacket( ReadArgs & args, uint32_t entityId );

		static size_t getRawSingleValueDataPacket( SingleValueDataPacket *singleValueDataPacket, uint8_t *raw );
		static size_t getRawAllValuesDataPacket( AllValuesDataPacket *allValuesDataPacket, uint8_t *raw );
	};
}