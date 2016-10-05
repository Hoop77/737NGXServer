#pragma once

#include "TCPStream.h"

namespace TCP
{
    class TCPAcceptor
    {
    public:
        TCPAcceptor( const string address, uint16_t port );
        ~TCPAcceptor();

        void start();
        TCPStream *accept();

    private:
        SOCKET listenSocket;
        const string address;
		uint16_t port;
        bool listening;
    };
}