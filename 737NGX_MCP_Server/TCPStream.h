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
        virtual ~Stream();

        // We don't want any copy or move construction/assignment since it's the factories' job to create a stream object.
        Stream( const Stream & other ) = delete;
        Stream & operator=( const Stream & other ) = delete;
        Stream( Stream && other ) = delete;
        Stream & operator=( Stream && other ) = delete;

        size_t write( const char *buffer, size_t len );
        size_t read( char *buffer, size_t len );
        void close();

        std::string getPeerIP() const;
        uint16_t getPeerPort() const;

    private:
        explicit Stream( SOCKET socket, const std::string & peerIP, uint16_t peerPort );
        
        SOCKET socket;
        std::string peerIP;
        uint16_t peerPort;
    };
}

