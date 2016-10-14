#include "CommandConnectionHandler.h"
#include "CommandServer.h"
#include "Packet.h"
#include "TCPException.h"
#include <string>


using namespace CommandHandling;


ConnectionHandler::ConnectionHandler( Server *server )
    : server( server )
    , runThread( nullptr )
    , running( false ) {}


ConnectionHandler::~ConnectionHandler()
{
    stop();
}


void ConnectionHandler::run()
{
    if( running )
        return;

    running = true;
    runThread.reset( new std::thread( [&]
    {
        // Get the queue from the server which we will permanently try to
        // extract streams from to receive new packets.
        auto streamQueue = server->getStreamQueue();
        char *packetBuffer = new char[ Protocol::Packet::MAX_SIZE ];

        while( 1 )
        {
            if( !running )
                break;

            try
            {
                // Remove a stream from the queue (max wait time: 1s).
                auto stream = std::move(
                    streamQueue->dequeue( std::chrono::seconds( 1 ) ) );

                try
                {
                    // Receive incoming command data.
                    size_t len = stream->read( packetBuffer, Protocol::Packet::MAX_SIZE );
                    if( len == 0 )
                    {
                        server->print( std::string( stream->getPeerIP() ).append( " has closed unexpectedly." ) );
                        break;
                    }

                    // Create packet from the received data.
                    Protocol::Packet commandPacket( packetBuffer, len );
                    Protocol::Packet::Method method = commandPacket.getMethod();
                    // Let the server either perform a SET or GET method according to the received packet.
                    if( method == Protocol::Packet::Method::SET )
                    {
                        server->performSetMethod( commandPacket );
                    }
                    else if( method == Protocol::Packet::Method::GET )
                    {
                        // The server will return a new packet to send to the client.
                        Protocol::Packet sendPacket = server->performGetMethod( commandPacket );
                        len = stream->write( sendPacket.getData(), sendPacket.getSize() );
                        if( len == 0 )
                        {
                            server->print( std::string( stream->getPeerIP() ).append( " has closed unexpectedly." ) );
                        }
                    }

                    stream->close();
                }
                catch( TCP::Exception & e )
                {
                    server->print( std::string( "Connection Error from: " ).append( stream->getPeerIP() ) );
                }
                catch( Protocol::PacketException & e )
                {
                    server->print( std::string( "Invalid Packet from: " ).append( stream->getPeerIP() ) );
                }
            }
            catch( Utils::TimeoutException & e )
            {
                // Just try again in case of timeout.
                continue;
            }
        }
    }));
}


void ConnectionHandler::stop()
{
    running = false;
    if( runThread != nullptr )
        runThread->join();
}
