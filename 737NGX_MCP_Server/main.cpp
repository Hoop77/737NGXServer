#include <iostream>
#include <string>

#include "SimConnectClient.h"
#include "MCPEntity.h"


using namespace std;

int main( int argc, char *argv[] )
{
	SimConnectClient simConnectClient;

	MCPEntity *mcpEntity = new MCPEntity( "Multi Control Panel Module" );
	simConnectClient.addEntity( mcpEntity );

	// connect to and run simconnect
	try
	{
        simConnectClient.connect();
		simConnectClient.run();
	}
	catch( const string &msg )
	{
		cout << msg << endl;
	}

	cin.get();
	
	return 0;
}


//#undef UNICODE
//
//
//#include "TCP.h"
//#include "TCPAcceptor.h"
//#include "TCPStream.h"
//#include "TCPException.h"
//#include <memory>
//#include <thread>
//#include <iostream>
//
//
//#include "global.h"
//
//
//int __cdecl main( void )
//{
//    TCP::init();
//
//    std::thread commandServer( []() 
//    {
//        TCP::Acceptor commandAcceptor( "127.0.0.1", 7654 );
//        commandAcceptor.start();
//
//        TCP::Stream *stream = commandAcceptor.accept();
//
//        size_t len;
//        char buf[ 256 ];
//        try 
//        {
//            while( (len = stream->read( buf, sizeof( buf ) )) > 0 )
//            {
//                std::cout << "Command received: " << buf << endl;
//            }
//        }
//        catch( TCP::Exception &e )
//        {
//            cout << e.what() << endl;
//        }
//
//        delete stream;
//    } );
//
//    std::thread updateServer( []() 
//    {
//        TCP::Acceptor updateAcceptor( "127.0.0.1", 7655 );
//        updateAcceptor.start();
//
//        TCP::Stream *stream = updateAcceptor.accept();
//
//        char *buf = "This is new Information!";
//        size_t len = strlen( buf ) + 1;
//        try
//        {
//            while( stream->write( buf, len ) > 0 )
//            {
//                std::this_thread::sleep_for( 2s );
//            }
//        }
//        catch( TCP::Exception &e )
//        {
//            cout << e.what() << endl;
//        }
//
//        delete stream;
//    } );
//
//    commandServer.join();
//    updateServer.join();
//
//    TCP::cleanup();
//
//    std::cout << "Done." << endl;
//
//    getchar();
//
//    return 0;
//}