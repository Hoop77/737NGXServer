#include "MCPEntity.h"
#include "SimConnectClient.h"
using namespace SimConnect;


MCPEntity::MCPEntity( const std::string & name )
	: Entity( name ) {}


void MCPEntity::setup()
{
	try
	{
		setupDataConnection();
		setupEvents();
	}
	catch( Exception & e )
	{
		// Simply rethrow.
		throw;
	}
}


void MCPEntity::dispatch( SIMCONNECT_RECV *data, DWORD size, void *context )
{
	if( data->dwID == SIMCONNECT_RECV_ID_EVENT )
	{
		SIMCONNECT_RECV_EVENT *event = (SIMCONNECT_RECV_EVENT *) data;

		if( event->uEventID == EVENT_KEYBOARD_A )
		{
			// slew heading selector
			SimConnect_TransmitClientEvent( simConnect, 0, EVENT_HEADING_SELECTOR, MOUSE_FLAG_WHEEL_UP,
				SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY );
		}
	}
}


void MCPEntity::close()
{

}


void MCPEntity::setupDataConnection()
{
	HRESULT result;

	// Associate an ID with the PMDG data area name
	result = SimConnect_MapClientDataNameToID( simConnect, PMDG_NGX_DATA_NAME, PMDG_NGX_DATA_ID );
	if( result == E_FAIL ) throw Exception( "SimConnect_MapClientDataNameToID" );

	// Define the data area structure - this is a required step
	result = SimConnect_AddToClientDataDefinition( simConnect, PMDG_NGX_DATA_DEFINITION, 0, sizeof( PMDG_NGX_Data ), 0, 0 );
	if( result == E_FAIL ) throw Exception( "SimConnect_AddToClientDataDefinition" );

	// Sign up for notification of data change.  
	// SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED flag asks for the data to be sent only when some of the data is changed.
	result = SimConnect_RequestClientData( simConnect, PMDG_NGX_DATA_ID, DATA_REQUEST, PMDG_NGX_DATA_DEFINITION,
		SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0 );
	if( result == E_FAIL ) throw Exception( "SimConnect_RequestClientData" );
}


void MCPEntity::setupEvents()
{
	HRESULT result;

	result = SimConnect_MapClientEventToSimEvent( simConnect, EVENT_HEADING_SELECTOR, "#70022" );
	if( result == E_FAIL ) throw Exception( "SimConnect_MapClientEventToSimEvent" );


	result = SimConnect_MapClientEventToSimEvent( simConnect, EVENT_KEYBOARD_A );
	if( result == E_FAIL ) throw Exception( "SimConnect_MapClientEventToSimEvent" );
}


void MCPEntity::setValueData( Global::ValueId::Type valueId, uint32_t valueData )
{
	SimConnect_TransmitClientEvent( simConnect, 0, valueId, valueData,
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY );
}


uint32_t MCPEntity::getValueData( Global::ValueId::Type valueId )
{
	return values[ valueId ].load();
}


void MCPEntity::setEntireData( const uint32_t *valueData )
{
	// Loop through all value IDs and set them accordingly.
	for( Global::ValueId::Type valueId = 0; 
		valueId < Global::ValueId::MCP::VALUE_COUNT; 
		valueId++ )
	{
		setValueData( valueId, valueData[ valueId ] );
	}
}


void MCPEntity::getEntireData( uint32_t *valueData )
{
	// Loop through all value IDs and get the data.
	for( Global::ValueId::Type valueId = 0; 
		valueId < Global::ValueId::MCP::VALUE_COUNT; 
		valueId++ )
	{
		valueData[ valueId ] = values[ valueId ].load();
	}
}