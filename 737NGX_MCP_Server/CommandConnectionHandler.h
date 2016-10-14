#pragma once


#include <thread>
#include "TCPStream.h"
#include "WorkQueue.h"
#include "CommandServer.h"


namespace CommandHandling
{
    class Server;

    class ConnectionHandler
    {
    public:
        explicit ConnectionHandler( Server *server );
        virtual ~ConnectionHandler();

        void run();
        void stop();

    private:
        Server *server;
        std::unique_ptr<std::thread> runThread;
        bool running;
    };
}

