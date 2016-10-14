#pragma once


#include "WorkQueue.h"
#include "TCPStream.h"
#include <string>
#include <vector>
#include <memory>
#include "CommandConnectionHandler.h"
#include "Packet.h"


namespace CommandHandling
{
    class ConnectionHandler;

    class Server
    {
    public:
        explicit Server( const std::string & ip, uint16_t port, size_t connectionHandlerCount );
        virtual ~Server();

        void run();
        void stop();

        void print( const std::string & msg );

        void performSetMethod( Protocol::Packet && packet );
        Protocol::Packet performGetMethod( Protocol::Packet && packet );

        Utils::WorkQueue<std::unique_ptr<TCP::Stream>> *getStreamQueue() const { return streamQueue.get(); }

    private:
        std::unique_ptr<Utils::WorkQueue<std::unique_ptr<TCP::Stream>>> streamQueue;
        std::vector<std::unique_ptr<ConnectionHandler>> connectionHandlers;
        size_t connectionHandlerCount;
        std::string ip;
        uint16_t port;
        std::mutex printMutex;
    };
}

