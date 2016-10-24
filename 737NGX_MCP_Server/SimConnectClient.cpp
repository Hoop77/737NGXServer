#include "SimConnectClient.h"
using namespace SimConnect;


Client::Client( std::shared_ptr<std::vector<std::unique_ptr<Entity>>> entities )
	: entities( entities )
	, running( false ) {}


void 
Client::connect()
{
	// open simconnect
	HRESULT result = SimConnect_Open( &simConnect, "737NGX Server", NULL, 0, 0, 0 );
	if( result == E_FAIL )
	{
		throw Exception( "Unable to connect to SimConnect!" );
	}

	// setup entities
	for( auto & entity : *entities )
	{
		entity->obtainSimConnectHandle( simConnect );
		try
		{
			entity->setup();
		}
		catch( const Exception & e )
		{
			SimConnect_Close( simConnect );
			std::cout << "Failed to setup with message: '" << e.what() << "'" << std::endl;
			throw Exception( "Failed to connect to Flight Simulator!" );
		}
	}

	std::cout << "Connected to Flight Simulator!\n" << std::endl;
}


void 
Client::run()
{
	if( running )
		return;

	running = true;

	// main loop
	while( running )
	{
		SimConnect_CallDispatch( simConnect, &Client::dispatch, this );

		Sleep( 1 );
	}

	// close entities
	for( auto & entity : *entities )
	{
		entity->close();
	}

	SimConnect_Close( simConnect );
	std::cout << "Disconnected from Flight Simulator." << std::endl;
}


void
Client::stop()
{
	running = true;
}


void CALLBACK
Client::dispatch( SIMCONNECT_RECV *data, DWORD size, void *context )
{
	Client *self = static_cast<Client *>(context);

	if( data->dwID == SIMCONNECT_RECV_ID_QUIT )
	{
		self->running = false;
	}
	else
	{
		// call each entity's dispatch routine
		for( auto & entity : *self->entities )
		{
			entity->dispatch( data, size, context );
		}
	}
}