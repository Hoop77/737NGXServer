#pragma once


#include "TCP.h"


#include <string>


namespace TCP
{
    class Stream
    {
        friend class Acceptor;
        friend class Connector;

    public:
        // No public constructor because a Stream can only be created by Acceptors and Connectors.

        virtual ~Stream();

        // Copy protection since destructor closes the socket.
        Stream( const Stream &other ) = delete;
        Stream &operator=( const Stream &other ) = delete;

        size_t write( const char *buffer, size_t len );
        size_t read( char *buffer, size_t len );

        std::string getPeerIP() const;
        uint16_t getPeerPort() const;

    private:
        explicit Stream( SOCKET socket, const std::string &peerIP, uint16_t peerPort );
        
        SOCKET socket;
        const std::string peerIP;
        const uint16_t peerPort;
    };
}

