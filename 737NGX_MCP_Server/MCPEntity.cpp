#include "MCPEntity.h"


MCPEntity::MCPEntity( string name )
    : SimConnectEntity( name ) {}


MCPEntity::~MCPEntity() {}


void MCPEntity::setup()
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
    if( result == E_FAIL ) throw "SimConnect_MapClientDataNameToID";

    // Define the data area structure - this is a required step
    result = SimConnect_AddToClientDataDefinition( simConnect, PMDG_NGX_DATA_DEFINITION, 0, sizeof( PMDG_NGX_Data ), 0, 0 );
    if( result == E_FAIL ) throw "SimConnect_AddToClientDataDefinition";

    // Sign up for notification of data change.  
    // SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED flag asks for the data to be sent only when some of the data is changed.
    result = SimConnect_RequestClientData( simConnect, PMDG_NGX_DATA_ID, DATA_REQUEST, PMDG_NGX_DATA_DEFINITION,
        SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0 );
    if( result == E_FAIL ) throw "SimConnect_RequestClientData";



    result = SimConnect_MapClientEventToSimEvent( simConnect, EVENT_HEADING_SELECTOR, "#70022" );
    if( result == E_FAIL ) throw "SimConnect_MapClientEventToSimEvent";


    result = SimConnect_MapClientEventToSimEvent( simConnect, EVENT_KEYBOARD_A );
    if( result == E_FAIL ) throw "SimConnect_MapClientEventToSimEvent";


    result = SimConnect_AddClientEventToNotificationGroup( simConnect, GROUP_KEYBOARD, EVENT_KEYBOARD_A );
    if( result == E_FAIL ) throw "SimConnect_AddClientEventToNotificationGroup";


    result = SimConnect_SetNotificationGroupPriority( simConnect, GROUP_KEYBOARD, SIMCONNECT_GROUP_PRIORITY_HIGHEST );
    if( result == E_FAIL ) throw "SimConnect_SetNotificationGroupPriority";


    result = SimConnect_MapInputEventToClientEvent( simConnect, INPUT, "shift+ctrl+a", EVENT_KEYBOARD_A );
    if( result == E_FAIL ) throw "SimConnect_MapInputEventToClientEvent";


    result = SimConnect_SetInputGroupState( simConnect, INPUT, SIMCONNECT_STATE_ON );
    if( result == E_FAIL ) throw "SimConnect_SetInputGroupState";
}


void MCPEntity::setupControlConnection()
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