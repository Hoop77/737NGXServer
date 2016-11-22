#include "TCP.h"
#include "Global.h"
#include "SimConnectClient.h"
#include "SimConnectEntity.h"
#include "SimConnectOnEntityValueChangedListener.h"
#include "MCPEntity.h"
#include "CommandHandlingServer.h"
#include "NotificationHandlingServer.h"
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


int main( int argc, char *argv[] )
{
	using namespace std;

	// Globally enable TCP.
	try
	{
		TCP::init();
	}
	catch( TCP::Exception & e )
	{
		cout << "Fatal: Could not initialize networking!" << endl;
	}

	// The entities vector.
	auto entities = make_shared<vector<unique_ptr<SimConnect::Entity>>>();

	// The MCP entity.
	auto mcpEntity = make_unique<SimConnect::MCPEntity>();

	// Add MCP entity to the entities vector.
	entities->push_back( std::move( mcpEntity ) );

	// Create the command handling server.
	CommandHandling::Server cmdServer( "127.0.0.1", 7654, 1, entities );

	// Create the notification handling server.
	NotificationHandling::Server notificationServer( "192.168.178.29", 7653 );

	// When values of entities are changed, the notification server has to broadcast these changes to its clients. 
	// Therefore we create a listener for each entity and that listeners will be called when
	// a single value of its entity is changed. When that happens we tell the server to broadcast that value
	// to all connected clients.
	auto mcpListener = make_shared<SimConnect::OnEntityValueChangedListener>(
				[&]( unsigned int valueId, uint32_t value )
	{
		notificationServer.broadcastNotification( Global::EntityId::MCP, valueId, value );
	} );

	// Register listeners.
	entities->at( Global::EntityId::MCP )->registerOnEntityValueChangedListener( mcpListener );

	// Create the simconnect client.
	SimConnect::Client simConnectClient( entities );

	// Indicates if we we're connected to simconnect.
	bool connected = false;

	// try to connect to simconnect
	try
	{
		simConnectClient.connect();
		connected = true;
	}
	catch( SimConnect::Exception & e )
	{
		cout << "Could not connect to Flight Simulator!" << endl;
	}

	if( connected )
	{
		// Thread for simconnect client.
		thread simConnectThread( [&]
		{
			simConnectClient.run();
		} );

		// Thread for command handling server.
		thread cmdThread( [&]
		{
			cmdServer.run();
		} );

		// Thread for notification handling server.
		thread notificationThread( [&]
		{
			notificationServer.run();
		} );

		// If the simconnect client stops running, we stop the servers and join.
		simConnectThread.join();
		cmdServer.stop();
		notificationServer.stop();
		cmdThread.join();
		notificationThread.join();

		// cleanup
		TCP::cleanup();

		cout << "Goodbye!" << endl;
	}

	cin.get();
	return 0;
}