#include "SimConnectClient.h"


using namespace std;


SimConnectClient::SimConnectClient()
    : quit( false ) {}


SimConnectClient::~SimConnectClient() {}


void SimConnectClient::addEntity( SimConnectEntity *entity )
{
    entities.push_back( entity );
}


void SimConnectClient::connect()
{
    // open simconnect
    HRESULT result = SimConnect_Open( &simConnect, "737NGX Server", NULL, 0, 0, 0 );
    if( result == E_FAIL )
    {
        throw SimConnectClientException( "Unable to connect to SimConnect!" );
    }

    // setup entities
    for( auto entity : entities )
    {
        entity->obtainSimConnectHandle( simConnect );
        try
        {
            entity->setup();
        }
        catch( const SimConnectEntityException & e )
        {
            SimConnect_Close( simConnect );
            std::cout << "Failed to setup '" << entity->getName() << "'" << " with message: '" << e.what() << "'" << std::endl;
            throw SimConnectClientException( "Failed to connect to Flight Simulator!" );
        }
    }

    std::cout << "Connected to Flight Simulator!\n" << std::endl;
}


void SimConnectClient::run()
{
    // main loop
    while( !quit )
    {
        SimConnect_CallDispatch( simConnect, &SimConnectClient::dispatch, this );

        Sleep( 1 );
    }

    // close entities
    for( auto entity : entities )
    {
        entity->close();
    }

    SimConnect_Close( simConnect );
    std::cout << "Disconnected from Flight Simulator." << std::endl;
}


void CALLBACK SimConnectClient::dispatch( SIMCONNECT_RECV *data, DWORD size, void *context )
{
    SimConnectClient *self = static_cast<SimConnectClient *>(context);

    if( data->dwID == SIMCONNECT_RECV_ID_QUIT )
    {
        self->quit = true;
    }
    else
    {
        // call each entity's dispatch routine
        for( auto entity : self->entities )
        {
            entity->dispatch( data, size, context );
        }
    }
}