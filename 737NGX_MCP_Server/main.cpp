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
#include <memory>
#include <thread>
#include <iostream>


int __cdecl main( void )
{
    TCP::init();

    TCP::Acceptor *acceptor = new TCP::Acceptor( "127.0.0.1", 7654 );

    acceptor->start();

    TCP::Stream *stream = acceptor->accept();

    std::thread readThread( [&] () {
        size_t len;
        char buf[ 256 ];
        while( len = stream->read( buf, 256 ) > 0 )
        {
            buf[ len ] = '\0';
            std::cout << "received: " << buf << endl;
        }
    } );

    std::thread writeThread( [&]() {
        char *buf = "Spam!";
        size_t len = strlen( buf ) + 1;
        while( len = stream->write( buf, len ) > 0 );
    } );

    readThread.join();
    writeThread.join();

    delete acceptor;

    TCP::cleanup();

    getchar();

    return 0;
}



