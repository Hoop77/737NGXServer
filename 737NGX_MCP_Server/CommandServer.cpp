#include "CommandServer.h"
#include <iostream>
#include "TCPAcceptor.h"
#include "TCPException.h"


using namespace CommandHandling;


Server::Server( const std::string & ip, uint16_t port, size_t connectionHandlerCount )
	: ip( ip )
	, port( port )
	, connectionHandlerCount( connectionHandlerCount )
{
	// Initializing the queue of TCP streams.
	streamQueue = std::unique_ptr<Utils::WorkQueue<std::unique_ptr<TCP::Stream>>>(
		new Utils::WorkQueue<std::unique_ptr<TCP::Stream>> );

	// Creating the specified number of connection handlers (we don't let them run at this point).
	for( size_t i = 0; i < connectionHandlerCount; i++ )
	{
		connectionHandlers.push_back(
			std::unique_ptr<ConnectionHandler>( new ConnectionHandler( this ) ) );
	}
}


Server::~Server()
{
	stop();
}


void Server::run()
{
	// Lets run the connection handlers.
	for( auto & connectionHandler : connectionHandlers )
	{
		connectionHandler->run();
	}

	try
	{
		// Accept incoming TCP connections and add them to the queue.
		TCP::Acceptor acceptor( ip, port );
		acceptor.start();

		while( 1 )
		{
			auto stream = acceptor.accept();
			streamQueue->enqueue( std::move( stream ) );
		}
	}
	catch( TCP::Exception & e )
	{
		message( std::string( "TCP Error: " ).append( e.what() ) );
	}
}


void Server::stop()
{
	// Stopping the connection handlers.
	for( auto & connectionHandler : connectionHandlers )
	{
		connectionHandler->stop();
	}
}


void Server::message( const std::string & msg )
{
	std::lock_guard<std::mutex> lock( printMutex );
	std::cout << msg << std::endl;
}


void Server::performSetMethod( Protocol::Packet && packet )
{

}


Protocol::Packet Server::performGetMethod( Protocol::Packet && packet )
{
	return packet;
}

