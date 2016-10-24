#include "NotificationHandlingServer.h"
#include "Global.h"
#include "TCPException.h"

#include <thread>


using namespace NotificationHandling;


Server::Server( const std::string & ip, uint16_t port )
	: acceptor( ip, port )
	, running( false ) {}


Server::~Server()
{
	try { stop(); }
	catch( ... ) {}
}


void 
Server::run()
{
	if( running )
		return;

	running = true;

	// Start the boradcasting thread.
	startBroadcastThread();

	// Accept and add incoming clients to the list of TCP streams.
	try
	{
		acceptor.start();

		while( running )
		{
			auto stream = acceptor.accept();
			// Lock the list to add the new stream.
			std::lock_guard<std::mutex> streamListLock( streamListMutex );
			streamList.push_back( std::move( stream ) );
		}
	}
	catch( TCP::Exception & e )
	{
		if( running )
			message( std::string( "TCP Error: " ).append( e.what() ) );
	}
}


void 
Server::startBroadcastThread()
{
	broadcastThread.reset( new std::thread( [&]
	{
		while( 1 )
		{
			if( !running )
				break;

			try
			{
				// Get packet to broadcast with timeout: 1s.
				auto packet = std::move( packetQueue.dequeue( 1000 ) );

				// Lock the list of streams.
				std::lock_guard<std::mutex> streamListLock( streamListMutex );

				// Write packet to every stream in the list.
				auto i = streamList.begin();
				while( i != streamList.end() )
				{
					auto & stream = *i;
					size_t size = stream->write( (char *) packet->getData(), packet->getSize() );
					// Stream closed?
					if( size == 0 )
					{
						// Remove the stream from the list which gets closed automatically.
						i = streamList.erase( i );
						continue;
					}

					++i;
				}
			}
			catch( Utils::TimeoutException & e )
			{
				continue;
			}
			catch( TCP::Exception & e )
			{
				message( std::string( "TCP Error: " ).append( e.what() ) );
			}
		}
	} ) );
}


void 
Server::stop()
{
	if( !running )
		return;

	running = false;

	try
	{
		// Stopping the blocking "accept()" method.
		acceptor.stop();

		if( broadcastThread != nullptr )
			broadcastThread->join();
	}
	catch( ... )
	{
		message( "Cannot stop notification handling server!\nIn order to close this application, use task manager!" );
	}
}


void 
Server::message( const std::string & msg )
{
	Global::println( msg );
}


void
Server::broadcastNotification( unsigned int entityId, unsigned int valueId, uint32_t value )
{
	using namespace Protocol;

	auto notificationPacket = PacketFactory::createSingleValueDataPacket( entityId, valueId, value );
	packetQueue.enqueue( std::move( notificationPacket ) );
}