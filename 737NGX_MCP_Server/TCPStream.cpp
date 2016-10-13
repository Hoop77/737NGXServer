#include "TCPStream.h"
#include "TCP.h"
#include "TCPException.h"
using namespace TCP;


Stream::Stream( SOCKET socket, const std::string & peerIP, uint16_t peerPort )
    : socket( socket )
    , peerIP( peerIP )
    , peerPort( peerPort ) {}


Stream::~Stream()
{
    closesocket( socket );
}


size_t
Stream::write( const char *buffer, size_t len )
{
    int result = ::send( socket, buffer, (int) len, 0 );

    if( result < 0 )
        throw Exception( "send failed" );

    return result;
}


size_t 
Stream::read( char *buffer, size_t len )
{
    int result = ::recv( socket, buffer, len, 0 );

    if( result < 0 )
        throw Exception( "recv failed" );

    return result;
}


std::string
Stream::getPeerIP() const
{
    return peerIP;
}


uint16_t 
Stream::getPeerPort() const
{
    return peerPort;
}
