//#include <iostream>
//#include <string>
//
//#include "SimConnectClient.h"
//#include "MCPEntity.h"
//
//#include "CommandInterpreter.h"
//#include "MCPInterpreter.h"
//
//#include "Commander.h"
//
//#include "TCPServer.h"
//
//using namespace std;
//
//int main( int argc, char *argv[] )
//{
//	SimConnectClient simConnectClient;
//	Commander commander;
//
//	//
//	// simconnect entities and interpreters
//	//
//	MCPEntity *mcpEntity = new MCPEntity( "Multi Control Panel Module" );
//	MCPInterpreter *mcpInterpreter = new MCPInterpreter( mcpEntity );
//
//	simConnectClient.addEntity( mcpEntity );
//	commander.addInterpreter( mcpInterpreter );
//
//	// connect to and run simconnect
//	try
//	{
//		simConnectClient.connect();
//		commander.startListening();
//
//		simConnectClient.run();
//
//		commander.stopListening();
//	}
//	catch( const string & msg )
//	{
//		cout << msg << endl;
//	}
//
//	cin.get();
//	
//	return 0;
//}


#undef UNICODE


#include "TCP.h"
#include "TCPAcceptor.h"
#include "TCPStream.h"


using namespace TCP;


int __cdecl main( void )
{
	TCP::init();

	TCPStream *stream = NULL;
	TCPAcceptor *acceptor = NULL;

	acceptor = new TCPAcceptor( "127.0.0.1", 7654 );

	acceptor->start();

	while( 1 )
	{
		stream = acceptor->accept();
		if( stream != NULL )
		{
			size_t len;
			char line[ 256 ];
			while( (len = stream->receive( line, sizeof( line ) )) > 0 )
			{
				line[ len ] = NULL;
				printf( "received - %s\n", line );
				stream->send( line, len );
			}
			delete stream;
		}
	}

	TCP::cleanup();

	getchar();

	return 0;
}



