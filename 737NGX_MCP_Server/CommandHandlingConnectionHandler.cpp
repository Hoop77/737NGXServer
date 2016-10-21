#include "CommandHandlingConnectionHandler.h"
#include "CommandHandlingServer.h"
#include "CommandHandlingException.h"
#include "Packet.h"
#include "PacketFactory.h"
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
		char packetBuffer[ Packet::MAX_SIZE ];

		while( 1 )
		{
			if( !running )
				break;

			try
			{
				// Remove a stream from the queue with timout 1s.
				auto stream = std::move( streamQueue->dequeue( 1000 ) );
				std::string ip( stream->getPeerIP() );

				try
				{
					// Receive incoming packet.
					size_t size = stream->read( packetBuffer, Packet::MAX_SIZE );
					if( size == 0 )
					{
						server.message( std::string( stream->getPeerIP() ).append( " has closed unexpectedly." ) );
						break;
					}

					// Create a packet from the received data.
					std::unique_ptr<Packet> receivedPacket =
						PacketFactory::createPacketFromReceivedData( packetBuffer, size );

					// Evaluate the packet type.
					int packetType = receivedPacket->getPacketType();
					// Event-Packet received?
					if( packetType == Packet::PACKET_TYPE_EVENT )
					{
						server.handleEventPacket( 
							static_cast<EventPacket *>( receivedPacket.get() ) );
					}
					// Request-Packet received?
					else if( packetType == Packet::PACKET_TYPE_REQUEST )
					{
						// The server will return a Data-Packet which we will transmit to the client.
						std::unique_ptr<DataPacket> transmitPacket(
							server.handleRequestPacket(
								static_cast<RequestPacket *>( receivedPacket.get() ) ) );

						size = stream->write( transmitPacket->getData(), transmitPacket->getSize() );
						if( size == 0 )
						{
							server.message( std::string( stream->getPeerIP() ).append( " has closed unexpectedly." ) );
						}
					}

					stream->close();
				}
				catch( TCP::Exception & e )
				{
					server.message( std::string( "Connection Error from: " ).append( stream->getPeerIP() ) );
				}
				catch( PacketException & e )
				{
					server.message( std::string( "Invalid Packet from: " ).append( stream->getPeerIP() ) );
				}
				catch( Exception & e )
				{
					server.message( e.what() );
				}
			}
			catch( Utils::TimeoutException & e )
			{
				// Just try again in case of timeout.
				continue;
			}
		}
	} ) );
}


void 
ConnectionHandler::stop()
{
	running = false;
	if( runThread != nullptr )
		runThread->join();
}
