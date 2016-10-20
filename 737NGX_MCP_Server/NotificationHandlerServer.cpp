#include "NotificationnHandlingServer.h"
#include "Global.h"

#include <thread>


using namespace NotificationHandling;


Server::Server( const std::string & ip, uint16_t port )
	: acceptor( ip, port )
	, running( false ) 
{
	acceptor.start();
}


Server::~Server()
{

}

// starts the connection handler threads
void 
Server::run();
{
	if( running )
		return;

	running = true;

	// Start the boradcasting thread.
	broadcastThread.reset( new std::thread( [&]
	{
		while( 1 )
		{
			if( !running )
				break;

			// Get packet to broadcast.
			auto packet = std::move( packetQueue->dequeue( 1000 ) );

			// Lock the list of streams.
			std::lock_guard<std::mutex> streamListLock( streamListMutex );

			// Write packet to every stream in the list.
			auto i = streamList->begin();
			while( i != streamList->end() )
			{
				auto & stream = *i;
				size_t size = stream->write( packet->getData(), packet->getSize() );
				// Stream closed?
				if( size == 0 )
				{
					// Remove the stream from the list which gets closed automatically.
					i = streamList->erase( i );
					continue;
				}

				++i;
			}
		}
	} ) );

	try
	{
		// Accept and add incoming clients to the list of TCP śtreams.
		while( 1 )
		{
			auto stream = acceptor.accept();
			// Lock the list.
			std::lock_guard<std::mutex> streamListLock( streamListMutex );
			streamList->push_back( std::move( stream ) );
		}
	}
	catch( TCP::Exception & e )
	{
		message( std::string( "TCP Error: " ).append( e.what() ) );
	}
}

// stops the connection handlers
void 
Server::stop()
{
	running = false;
	if( broadcastThread != nullptr )
		broadcastThread->join();
}

// prints out a message (able to do it asynchronously)
void 
Server::message( const std::string & msg )
{
	Global::println( msg );
}

// transmit a data packet to the clients containing a value that has changed
void
Server::broadcastNotification( unsigned int entityId, unsigned int valueId, uint32_t value )
{

}