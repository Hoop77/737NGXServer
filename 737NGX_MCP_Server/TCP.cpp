#include "TCP.h"


namespace TCP
{
	void init()
	{
		WSADATA wsaData;
		int result = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
		if( result != 0 )
			throw TCPException( "WSAStartup failed" );
	}


	void cleanup()
	{
		WSACleanup();
	}
}