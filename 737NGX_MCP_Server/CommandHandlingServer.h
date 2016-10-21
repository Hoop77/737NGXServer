#pragma once


#include "WorkQueue.h"
#include "TCPStream.h"
#include "TCPAcceptor.h"
#include <string>
#include <vector>
#include <memory>
#include "CommandHandlingConnectionHandler.h"
#include "Packet.h"
#include "SimConnectEntity.h"


namespace CommandHandling
{
	class ConnectionHandler;

	class Server
	{
	public:
		Server(
			const std::string & ip,
			uint16_t port,
			size_t connectionHandlerCount,
			std::shared_ptr<std::vector<std::unique_ptr<SimConnect::Entity>>> entities
		);

		~Server();

		// blocking - starts the connection handler threads and permanently accepts TCP client connections
		void run();

		// stops the connection handlers
		void stop();

		// prints out a message (able to do it asynchronously)
		void message( const std::string & msg );

		// transmit incoming events to an entity
		void handleEventPacket( Protocol::EventPacket *eventPacket );

		// returns client requested data
		std::unique_ptr<Protocol::DataPacket> handleRequestPacket( Protocol::RequestPacket *requestPacket );

		// get pointer to the streams queue
		Utils::WorkQueue<std::unique_ptr<TCP::Stream>> *getStreamQueue() const { return streamQueue.get(); }

	private:
		bool running;

		// simconnect entities
		std::shared_ptr<std::vector<std::unique_ptr<SimConnect::Entity>>> entities;

		// work queue of streams to handle client connections
		std::unique_ptr<Utils::WorkQueue<std::unique_ptr<TCP::Stream>>> streamQueue;

		// connection handlers
		std::vector<std::unique_ptr<ConnectionHandler>> connectionHandlers;
		size_t connectionHandlerCount;

		// accepts incoming TCP client connections to create streams
		TCP::Acceptor acceptor;

		// mutex to handle async printing
		std::mutex printMutex;
	};
}

