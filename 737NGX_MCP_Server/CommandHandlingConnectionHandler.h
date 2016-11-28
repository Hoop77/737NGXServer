#pragma once


#include <thread>
#include "TCPStream.h"
#include "WorkQueue.h"
#include "CommandHandlingServer.h"
#include "Packet.h"


namespace CommandHandling
{
	class Server;

	class ConnectionHandler
	{
	public:
		explicit ConnectionHandler( Server & server );
		virtual ~ConnectionHandler();

		void run();
		void stop();

	private:
		void handleReceivedPacket( Protocol::Packet *receivedPacket, TCP::Stream *stream );

		Server & server;
		std::unique_ptr<std::thread> runThread;
		bool running;
	};
}

