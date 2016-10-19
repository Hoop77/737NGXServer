#include "TCP.h"
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


int main( int argc, char *argv[] )
{
	using namespace std;

	// globally enable TCP
	try
	{
		TCP::init();
	}
	catch( TCP::Exception & e )
	{
		cout << "Fatal: Could not initialize networking!" << endl;
	}

	// the entities vector
	shared_ptr<vector<unique_ptr<SimConnect::Entity>>> entities(
		new vector<unique_ptr<SimConnect::Entity>> );

	// the MCP entity
	unique_ptr<SimConnect::Entity> mcpEntity(
		new SimConnect::MCPEntity( "Multi Control Panel" ) );

	// add MCP entity to the entities vector
	entities->push_back( std::move( mcpEntity ) );

	// create the server
	CommandHandling::Server server( "127.0.0.1", 7654, 1, entities );

	// create simconnect client
	SimConnect::Client simConnectClient( entities );

	// try to connect to simconnect
	try
	{
		simConnectClient.connect();
	}
	catch( SimConnect::Exception & e )
	{
		cout << "Could not connect to Flight Simulator!" << endl;
		return 0;
	}

	// create thread for simconnect client
	thread simConnectThread( [&]
	{
		simConnectClient.run();
	} );

	// create thread for server
	thread serverThread( [&]
	{
		server.run();
	} );

	server.run();

	// cleanup
	TCP::cleanup();

	cin.get();
	return 0;
}