#include "CommandHandlingConnectionHandler.h"
#include "CommandHandlingServer.h"
#include "CommandHandlingException.h"
#include "Packet.h"
#include "PacketIO.h"
#include "ProtocolException.h"
#include "TCPException.h"
#include <string>


using namespace CommandHandling;


ConnectionHandler::ConnectionHandler( Server & server )
	: server( server )
	, running( false ) {}


ConnectionHandler::~ConnectionHandler()
{
	try { stop(); }
	catch( ... ) {}
}


void 
ConnectionHandler::run()
{
	using namespace Protocol;

	if( running )
		return;

	running = true;
	runThread.reset( new std::thread( [&]
	{
		// Get the queue from the server. 
		// To receive packets, we will permanently try to extract streams from the queue.
		auto streamQueue = server.getStreamQueue();

		while( 1 )
		{
			if( !running )
				break;

			try
			{
				// Remove a stream from the queue with timout 1s.
				auto stream = streamQueue->dequeue( 1000 );

				// Read incoming packet.
				auto receivedPacket = PacketIO::readPacketFromStream( stream.get() );

				handleReceivedPacket( receivedPacket.get(), stream.get() );

				stream->close();
			}
			catch( Utils::TimeoutException & e )
			{
				// Just keep trying again in case of timeout.
			}
			catch( Protocol::Exception & e )
			{
				server.message( e.what() );
			}
			catch( TCP::Exception & e )
			{
				server.message( "TCP error: " + e.what() );
			}
		}
	} ) );
}


void
ConnectionHandler::handleReceivedPacket( Protocol::Packet *receivedPacket, TCP::Stream *stream )
{
	using namespace Protocol;

	try
	{
		// Evaluate the packet type.
		int packetType = receivedPacket->getPacketType();
		// Event-Packet received?
		if( packetType == PacketType::EVENT )
		{
			// Send quick response.
			uint8_t response = Response::OK;
			stream->write( (char *) &response, 1 );

			server.handleEventPacket(
				dynamic_cast<EventPacket *>( receivedPacket) );
		}
		// Request-Packet received?
		else if( packetType == PacketType::REQUEST )
		{
			// The server will return a Data-Packet which we will transmit to the client.
			auto transmitPacket = server.handleRequestPacket(
					dynamic_cast<RequestPacket *>( receivedPacket ) );

			PacketIO::writePacketToStream( transmitPacket.get(), stream );
		}
	}
	catch( TCP::Exception & e )
	{
		throw;
	}
	catch( Protocol::Exception & e )
	{
		throw;
	}
}


void 
ConnectionHandler::stop()
{
	running = false;
	if( runThread != nullptr )
		runThread->join();
}
