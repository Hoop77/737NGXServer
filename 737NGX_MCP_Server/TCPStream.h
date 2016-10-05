#pragma once


#include <iostream>
#include <string>


#include "TCP.h"


using namespace std;

namespace TCP
{
    class TCPStream
    {
        friend class TCPAcceptor;
        friend class TCPConnector;

    public:
        virtual ~TCPStream();

        size_t send( const char *buffer, size_t len );
        size_t receive( char *buffer, size_t len );

        string getPeerIP();
        uint16_t getPeerPort();

    private:
        TCPStream( SOCKET socket, const string peerIP, uint16_t peerPort );

        SOCKET socket;
        const string peerIP;
        uint16_t peerPort;
    };
}

