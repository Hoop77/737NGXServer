#include "TCPStream.h"


using namespace TCP;


TCPStream::TCPStream( SOCKET socket, const string peerIP, uint16_t peerPort )
	: socket( socket )
	, peerIP( peerIP )
	, peerPort( peerPort ) {}


TCPStream::~TCPStream()
{
    closesocket( socket );
}


size_t TCPStream::send( const char *buffer, size_t len )
{
    int result = ::send( socket, buffer, (int)len, 0 );

	if( result < 0 )
		throw TCPException( "send failed" );
	
	return result;
}


size_t TCPStream::receive( char *buffer, size_t len )
{
	int result = ::recv( socket, buffer, len, 0 );

	if( result < 0 )
		throw TCPException( "recv failed" );
	
	return result;
}


string TCPStream::getPeerIP()
{
    return peerIP;
}


uint16_t TCPStream::getPeerPort()
{
    return peerPort;
}
