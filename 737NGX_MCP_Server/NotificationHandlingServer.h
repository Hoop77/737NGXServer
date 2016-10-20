#pragma once

#include "Utils.h"
#include "TCP.h"
#include "TCPStream.h"
#include "TCPAcceptor.h"


#include <list>
#include <memory>
#include <mutex>


namespace NotificationHandling
{
	class Server
	{
	public:
		Server( const std::string & ip, uint16_t port );

		~Server();

		// starts the connection handler threads
		void run();

		// stops the connection handlers
		void stop();

		// prints out a message (able to do it asynchronously)
		void message( const std::string & msg );

		// transmit a data packet to the clients containing a value that has changed
		void broadcastNotification( unsigned int entityId, unsigned int valueId, uint32_t value );

	private:
		std::list<std::unique_ptr<TCP::Stream>> streamList;
		std::mutex streamListMutex;

		Utils::WorkQueue<std::unique_ptr<Protocol::SingleValueDataPacket>> packetQueue;

		std::unique_ptr<std::thread> broadcastThread;

		TCP::Acceptor acceptor;

		bool running;
	}
}