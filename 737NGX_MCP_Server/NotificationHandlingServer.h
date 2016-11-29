#pragma once

#include "WorkQueue.h"
#include "TCP.h"
#include "TCPStream.h"
#include "TCPAcceptor.h"
#include "Packet.h"


#include <list>
#include <memory>
#include <mutex>


namespace NotificationHandling
{
	class Server
	{
	public:
		Server( const std::string & hostname, uint16_t port );

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
		void startBroadcastThread();

		// Helper function to send a value through a single stream and tries to receive response.
		// Returns false if client does not respond.
		bool sendValue( TCP::Stream *stream, Protocol::SingleValueDataPacket *packet );

		// streams to broadcast notifications to
		std::list<std::unique_ptr<TCP::Stream>> streamList;

		// adding and removing streams gets done in different threads so we need a mutex
		std::mutex streamListMutex;

		// packets (the notification data) to broadcast
		Utils::WorkQueue<std::unique_ptr<Protocol::SingleValueDataPacket>> packetQueue;

		std::unique_ptr<std::thread> broadcastThread;

		TCP::Acceptor acceptor;

		bool running;
	};
}