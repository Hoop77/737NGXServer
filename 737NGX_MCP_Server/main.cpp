#include "TCP.h"
#include "Global.h"
#include "SimConnectClient.h"
#include "SimConnectEntity.h"
#include "MCPEntity.h"
#include "CommandHandlingServer.h"
#include "TCPException.h"
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>


namespace Global
{
	static std::mutex printMutex;

	void println( const std::string & msg )
	{
		std::lock_guard<std::mutex> printLock( printMutex );
		std::cout << msg << std::endl;
	}
}


//int main( int argc, char *argv[] )
//{
//	using namespace std;
//
//	// Globally enable TCP.
//	try
//	{
//		TCP::init();
//	}
//	catch( TCP::Exception & e )
//	{
//		cout << "Fatal: Could not initialize networking!" << endl;
//	}
//
//	// The entities vector.
//	shared_ptr<vector<unique_ptr<SimConnect::Entity>>> entities(
//		new vector<unique_ptr<SimConnect::Entity>> );
//
//	// The MCP entity.
//	unique_ptr<SimConnect::Entity> mcpEntity(
//		new SimConnect::MCPEntity() );
//
//	// Add MCP entity to the entities vector.
//	entities->push_back( std::move( mcpEntity ) );
//
//	// Create the server.
//	CommandHandling::Server server( "127.0.0.1", 7654, 1, entities );
//
//	// create simconnect client
//	SimConnect::Client simConnectClient( entities );
//
//	// Indicates if we we're connected to simconnect.
//	bool connected = false;
//
//	// try to connect to simconnect
//	try
//	{
//		simConnectClient.connect();
//		connected = true;
//	}
//	catch( SimConnect::Exception & e )
//	{
//		cout << "Could not connect to Flight Simulator!" << endl;
//	}
//
//	if( connected )
//	{
//		// create thread for simconnect client
//		thread simConnectThread( [&]
//		{
//			simConnectClient.run();
//		} );
//
//		// create thread for server
//		thread serverThread( [&]
//		{
//			server.run();
//		} );
//
//		// If the simconnect clients stops running, we stop the server and join.
//		simConnectThread.join();
//		server.stop();
//		serverThread.join();
//
//		// cleanup
//		TCP::cleanup();
//
//		cout << "Goodbye!" << endl;
//	}
//
//	cin.get();
//	return 0;
//}