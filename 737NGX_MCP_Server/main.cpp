#include "SimConnectClient.h"
#include "MCPEntity.h"
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <thread>


int main( int argc, char *argv[] )
{
	using namespace std;

	// the entities vector
	shared_ptr<vector<unique_ptr<SimConnect::Entity>>> entities;

	// the MCP entity
	unique_ptr<SimConnect::Entity> mcpEntity(
		new MCPEntity( "Multi Control Panel" ) );

	// add MCP entity to the entities vector
	entities->push_back( move( mcpEntity ) );

	// create the server
	CommandHandling::Server server( "127.0.0.1", 7654, 1, entities );

	// create simconnect client
	SimConnect::Client simConnectClient( entities );

	// connect to simconnect
	simConnectClient.connect();

	// create thread for simconnect client
	thread simConnectThread( [&]
	{
		simConnectClient.start();
	} );

	// create thread for server
	thread serverThread( [&]
	{
		server.run();
	} );

	std::cin.get();
	return 0;
}