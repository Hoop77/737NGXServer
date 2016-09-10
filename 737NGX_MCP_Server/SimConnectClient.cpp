#include "SimConnectClient.h"


// static
vector<SimConnectEntity *> SimConnectClient::entities;
bool SimConnectClient::quit = false;


void SimConnectClient::addEntity( SimConnectEntity *entity )
{
	entities.push_back( entity );
}


void SimConnectClient::connect()
{
	HANDLE simConnect = NULL;
	const string errorMsg = "Unable to connect to SimConnect!";

	// open simconnect
	HRESULT result = SimConnect_Open(
		&simConnect,
		"737NGX Server",
		NULL,
		0,
		0,
		0 );

	if( result == E_FAIL ) throw errorMsg;

	// setup entities
	for( auto entity : entities )
	{
		entity->obtainSimConnectHandle( simConnect );
		try
		{
			entity->setup();
		}
		catch( const string & msg )
		{
			cout << "Failed to setup '" << entity->getName() << "'" << " with message: '" << msg << "'" << endl;
			throw "Failed to connect to Flight Simulator!";
		}
	}

	cout << "Connected to Flight Simulator!\n" << endl;

	// main loop
	while( !quit )
	{
		SimConnect_CallDispatch( simConnect, &SimConnectClient::dispatch, NULL );

		Sleep( 1 );
	}
	
	// close entities
	for( auto entity : entities )
	{
		entity->close();
	}

	SimConnect_Close( simConnect );
	cout << "Disconnected from Flight Simulator." << endl;
}


void CALLBACK SimConnectClient::dispatch( SIMCONNECT_RECV *data, DWORD size, void *context )
{
	if( data->dwID == SIMCONNECT_RECV_ID_QUIT )
	{
		quit = true;
	}
	else
	{
		// call each entity's dispatch routine
		for( auto entity : entities )
		{
			entity->dispatch( data, size, context );
		}
	}
}