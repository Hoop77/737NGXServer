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
	using namespace Global::ValueId::MCP;

	try
	{
		mapValueIdToSimEvent( EVT_MCP_COURSE_SELECTOR_L, 	COURSE_L );
		mapValueIdToSimEvent( EVT_MCP_COURSE_SELECTOR_L + 1,COURSE_R );
		mapValueIdToSimEvent( EVT_MCP_FD_SWITCH_L, 			FD_SW_L );
		mapValueIdToSimEvent( EVT_MCP_FD_SWITCH_L + 1,		FD_SW_R );
		mapValueIdToSimEvent( EVT_MCP_AT_ARM_SWITCH, 		AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_N1_SWITCH, 			ANNUN_N1 );
		mapValueIdToSimEvent( EVT_MCP_SPEED_SWITCH, 		ANNUN_SPEED );
		mapValueIdToSimEvent( EVT_MCP_CO_SWITCH, 			AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_SPEED_SELECTOR, 		AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_VNAV_SWITCH, 			AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_SPD_INTV_SWITCH, 		AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_BANK_ANGLE_SELECTOR, 	AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_HEADING_SELECTOR, 	AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_LVL_CHG_SWITCH, 		AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_HDG_SEL_SWITCH, 		AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_APP_SWITCH, 			COURSE_L );
		mapValueIdToSimEvent( EVT_MCP_ALT_HOLD_SWITCH, 		COURSE_R );
		mapValueIdToSimEvent( EVT_MCP_VS_SWITCH, 			FD_SW_L );
		mapValueIdToSimEvent( EVT_MCP_VOR_LOC_SWITCH, 		FD_SW_R );
		mapValueIdToSimEvent( EVT_MCP_LNAV_SWITCH, 			AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_ALTITUDE_SELECTOR, 	AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_VS_SELECTOR, 			COURSE_L );
		mapValueIdToSimEvent( EVT_MCP_CMD_A_SWITCH, 		COURSE_R );
		mapValueIdToSimEvent( EVT_MCP_CMD_B_SWITCH, 		FD_SW_L );
		mapValueIdToSimEvent( EVT_MCP_CWS_A_SWITCH, 		FD_SW_R );
		mapValueIdToSimEvent( EVT_MCP_CWS_B_SWITCH, 		AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_DISENGAGE_BAR, 		AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_FD_SWITCH_R, 			AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_COURSE_SELECTOR_R, 	AT_ARM_SW );
		mapValueIdToSimEvent( EVT_MCP_ALT_INTV_SWITCH, 		AT_ARM_SW );
	}
	catch( Exception & e )
	{
		throw;
	}
}


void MCPEntity::setValueData( Global::ValueId::Type valueId, uint32_t valueData )
{
	// Check if the id is valid.
	if( !Global::ValueId::MCP::isValid( valueId ) )
	{
		throw Exception( "invalid id" );
	}

	SimConnect_TransmitClientEvent( simConnect, 0, valueId, valueData,
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY );
}


uint32_t MCPEntity::getValueData( Global::ValueId::Type valueId )
{
	// Check if the id is valid.
	if( !Global::ValueId::MCP::isValid( valueId ) )
	{
		throw Exception( "invalid id" );
	}

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