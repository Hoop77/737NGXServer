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
	// Close the socket on destruction.
	try { close(); }
	catch( ... ) {}
}


void
Stream::write( const char *buffer, size_t len )
{
    int result = send( socket, buffer, (int) len, 0 );

    if( result != len )
        throw Exception( "send failed" );
}


size_t 
Stream::read( char *buffer, size_t len )
{
    int result = recv( socket, buffer, len, 0 );

    if( result < 0 )
        throw Exception( "recv failed" );

    return result;
}


void 
Stream::close()
{
    if( socket == INVALID_SOCKET )
        return;

    int result = closesocket( socket );
    if( result != 0 )
        throw Exception( "closesocket failed" );

    socket = INVALID_SOCKET;
}


void
Stream::setTCPNoDelay( bool opt )
{
	// Use old C-style BOOL for compatibility.
	BOOL flag;
	if( opt )
		flag = TRUE;
	else
		flag = FALSE;

	int result = setsockopt( 
		socket,
		IPPROTO_TCP,
		TCP_NODELAY,
		(char *) &flag,
		sizeof( BOOL ) 
	);

	if( result != 0 )
		throw Exception( "setsockopt failed" );
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
