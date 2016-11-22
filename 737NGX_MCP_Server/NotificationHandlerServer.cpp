#include "NotificationHandlingServer.h"
#include "Global.h"
#include "TCPException.h"

#include <thread>


using namespace NotificationHandling;


Server::Server( const std::string & hostname, uint16_t port )
	: acceptor( hostname, port )
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
			message( stream->getPeerIP().append( " connected to notification server." ) );
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
	using namespace Protocol;

	broadcastThread.reset( new std::thread( [&]
	{
		while( 1 )
		{
			if( !running )
				break;

			try
			{
				// Get packet to broadcast with timeout: 1s.
				auto packet = packetQueue.dequeue( 1000 );

				// Lock the list of streams.
				std::lock_guard<std::mutex> streamListLock( streamListMutex );

				// Write packet to every stream in the list.
				auto i = streamList.begin();
				while( i != streamList.end() )
				{
					auto stream = i->get();
					bool success = sendValue( stream, static_cast<SingleValueDataPacket *>( packet.get() ) );
					if( !success )
					{
						// Remove the stream from the list which gets closed automatically.
						message( stream->getPeerIP().append( " disconnected from notification server." ) );
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
		}
	} ) );
}


bool
Server::sendValue( TCP::Stream *stream, Protocol::SingleValueDataPacket *packet )
{
	bool success = true;

	// Send value.
	stream->write( (char *) packet->getData(), packet->getSize() );

	// Try to receive response.
	try
	{
		uint8_t response = RESPONSE_ERROR;
		size_t size = stream->read( (char *) &response, 1 );
		// Check response
		if( size != 1 || response != RESPONSE_OK )
		{
			success = false;
		}
	}
	catch( TCP::Exception & e )
	{
		success = false;
	}

	return success;
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