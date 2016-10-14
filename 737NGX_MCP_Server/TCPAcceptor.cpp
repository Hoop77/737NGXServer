#include "TCPAcceptor.h"
#include "TCP.h"
#include "TCPException.h"
#include "TCPStream.h"
using namespace TCP;


Acceptor::Acceptor( const std::string & address, uint16_t port )
    : listenSocket( INVALID_SOCKET )
    , address( address )
    , port( port )
    , listening( false ) {}


Acceptor::~Acceptor()
{
    if( listenSocket != INVALID_SOCKET )
    {
        closesocket( listenSocket );
    }
}


void 
Acceptor::start()
{
    int iResult;

    // Nothing to do if already listening.
    if( listening )
    {
        return;
    }

    // We use addrinfo structure to resolve the adress and port.
    struct addrinfo *addrinfoResult = NULL;
    struct addrinfo hints;

    // Setting up family, socket type, protocol and flags.
    ZeroMemory( &hints, sizeof( hints ) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port.
    if( address.size() == 0 )
    {
        // Any IP address.
        iResult = getaddrinfo( NULL, std::to_string( port ).c_str(), &hints, &addrinfoResult );
    }
    else
    {
        // IP address specified.
        iResult = getaddrinfo( address.c_str(), std::to_string( port ).c_str(), &hints, &addrinfoResult );
    }

    if( iResult != 0 )
    {
        throw Exception( "getaddrinfo failed" );
    }

    // Create a SOCKET for connecting to server.
    listenSocket = socket( addrinfoResult->ai_family, addrinfoResult->ai_socktype, addrinfoResult->ai_protocol );
    if( listenSocket == INVALID_SOCKET )
    {
        freeaddrinfo( addrinfoResult );
        throw Exception( "socket failed" );
    }

    // Normally when you stop a server listening on a given IP address and port, 
    // it takes a few seconds before you can starting listening on the same IP address and port when you restart your server. 
    // To disable this condition and make it possible to immediately resue a listening port, 
    // we set the SO_REUSEADDR socket option for the listening socket before calling bind().
    int optval = 1;
    setsockopt( listenSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof( optval ) );

    // Bind the listening socket address to the socket descriptor.
    iResult = bind( listenSocket, addrinfoResult->ai_addr, (int) addrinfoResult->ai_addrlen );
    if( iResult == SOCKET_ERROR )
    {
        freeaddrinfo( addrinfoResult );
        closesocket( listenSocket );
        throw Exception( "bind failed" );
    }

    // We can free the addrinfo structure now.
    freeaddrinfo( addrinfoResult );

    // Start listening.
    iResult = listen( listenSocket, SOMAXCONN );
    if( iResult == SOCKET_ERROR )
    {
        closesocket( listenSocket );
        throw Exception( "listen failed" );
    }

    listening = true;
}


std::unique_ptr<Stream>
Acceptor::accept()
{
    // We have to be listening in order to accept a socket.
    if( !listening )
    {
        throw Exception( "not listening" );
    }

    // sockaddr_in structure to receive address information of the client.
    struct sockaddr_in address;
    socklen_t len = sizeof( address );
    memset( &address, 0, sizeof( address ) );

    // Call accept.
    SOCKET acceptSocket = ::accept( listenSocket, (struct sockaddr *) &address, &len );
    if( acceptSocket == INVALID_SOCKET )
    {
        throw Exception( "accept failed" );
    }

    // Extract ascii string representation of the sockaddr_in structure.
    char ip4String[ 50 ];
    inet_ntop(
        PF_INET,                                    // ip4 gets returned
        (struct in_addr *) &(address.sin_addr),     // pointer to the "in_addr"-structure
        ip4String,                                  // the string-buffer 
        sizeof( ip4String ) - 1                     // size of the string buffer
    );

    // Return new TCP Stream.
    return std::unique_ptr<Stream>(
        new Stream( acceptSocket, std::string( ip4String ), address.sin_port ) );
}