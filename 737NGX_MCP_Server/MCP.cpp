#include "MCP.h"



MCP::MCP( const string & name )
	: SimConnectEntity( name ) {}


MCP::~MCP() {}


void MCP::setup()
{
	try
	{
		setupDataConnection();
		setupControlConnection();
	}
	catch( const string & msg )
	{
		// pass through error message
		throw msg;
	}
}


void MCP::dispatch( SIMCONNECT_RECV *data, DWORD size, void *context )
{
	
}


void MCP::close()
{
	
}


void MCP::setupDataConnection()
{
	HRESULT result;

	// Associate an ID with the PMDG data area name
	result = SimConnect_MapClientDataNameToID( simConnect, PMDG_NGX_DATA_NAME, PMDG_NGX_DATA_ID );
	if( result == E_FAIL ) throw "SimConnect_MapClientDataNameToID";

	// Define the data area structure - this is a required step
	result = SimConnect_AddToClientDataDefinition( simConnect, PMDG_NGX_DATA_DEFINITION, 0, sizeof( PMDG_NGX_Data ), 0, 0 );
	if( result == E_FAIL ) throw "SimConnect_AddToClientDataDefinition";

	// Sign up for notification of data change.  
	// SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED flag asks for the data to be sent only when some of the data is changed.
	result = SimConnect_RequestClientData( simConnect, PMDG_NGX_DATA_ID, DATA_REQUEST, PMDG_NGX_DATA_DEFINITION,
		SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0 );
	if( result == E_FAIL ) throw "SimConnect_RequestClientData";
}


void MCP::setupControlConnection()
{
	HRESULT result;

	// First method: control data area
	control.Event = 0;
	control.Parameter = 0;

	// Associate an ID with the PMDG control area name
	result = SimConnect_MapClientDataNameToID( simConnect, PMDG_NGX_CONTROL_NAME, PMDG_NGX_CONTROL_ID );
	if( result == E_FAIL ) throw "SimConnect_MapClientDataNameToID";

	// Define the control area structure - this is a required step
	result = SimConnect_AddToClientDataDefinition( simConnect, PMDG_NGX_CONTROL_DEFINITION, 0, sizeof( PMDG_NGX_Control ), 0, 0 );
	if( result == E_FAIL ) throw "SimConnect_AddToClientDataDefinition";

	// Sign up for notification of control change.  
	result = SimConnect_RequestClientData( simConnect, PMDG_NGX_CONTROL_ID, CONTROL_REQUEST, PMDG_NGX_CONTROL_DEFINITION,
		SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0 );
	if( result == E_FAIL ) throw "SimConnect_RequestClientData";
}