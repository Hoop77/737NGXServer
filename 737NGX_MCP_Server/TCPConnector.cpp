#include "TCPConnector.h"


using namespace TCP;


TCPStream *TCPConnector::connect( const string server, uint16_t port )
{
	int iResult;
	SOCKET connectSocket;

	struct addrinfo *addrinfoResult = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	ZeroMemory( &hints, sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve server address and port.
	iResult = getaddrinfo(
		server.c_str(),				// server name
		to_string( port ).c_str(),	// port
		&hints,						// hints
		&addrinfoResult				// result
	);

	if( iResult != 0 )
	{
		throw TCPException( "getaddrinfo failed" );
	}

	// Attempt to connect to an address until one succeseds.
	for( ptr = addrinfoResult; ptr != NULL; ptr = ptr->ai_next )
	{
		// Create a SOCKET for connecting to server.
		connectSocket = socket(
			ptr->ai_family,
			ptr->ai_socktype,
			ptr->ai_protocol
		);

		if( connectSocket == INVALID_SOCKET )
		{
			freeaddrinfo( addrinfoResult );
			throw TCPException( "socket failed" );
		}

		// Connect to server.
		iResult = ::connect(
			connectSocket,
			ptr->ai_addr,
			(int) ptr->ai_addrlen
		);

		if( iResult == SOCKET_ERROR )
		{
			closesocket( connectSocket );
			connectSocket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	if( connectSocket == INVALID_SOCKET )
	{
		freeaddrinfo( addrinfoResult );
		throw TCPException( "unable to connect to server" );
	}

	// Create new TCPStream object.
	TCPStream *tcpStream = new TCPStream( connectSocket, port, server );

	// Throw addressinfo data away.
	freeaddrinfo( addrinfoResult );

	return tcpStream;
}