#include "MCPEntity.h"
#include "SimConnectClient.h"
using namespace SimConnect;


#include "Converting.h"


MCPEntity::MCPEntity() : Entity() {}


void 
MCPEntity::setup()
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


void 
MCPEntity::dispatch( SIMCONNECT_RECV *data, DWORD size, void *context )
{
	if( data->dwID == SIMCONNECT_RECV_ID_CLIENT_DATA )
	{
		SIMCONNECT_RECV_CLIENT_DATA *clientData = (SIMCONNECT_RECV_CLIENT_DATA *) data;

		if( clientData->dwRequestID == DATA_REQUEST_ID )
		{
			PMDG_NGX_Data *ngxData = (PMDG_NGX_Data *) &clientData->dwData;
			processNgxData( ngxData );
		}
	}
}


void 
MCPEntity::close()
{

}


void 
MCPEntity::setupDataConnection()
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
	result = SimConnect_RequestClientData( simConnect, PMDG_NGX_DATA_ID, DATA_REQUEST_ID, PMDG_NGX_DATA_DEFINITION,
		SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0 );
	if( result == E_FAIL ) throw Exception( "SimConnect_RequestClientData" );
}


void 
MCPEntity::setupEvents()
{
	using namespace Global::EventId::MCP;

	try
	{
		mapClientEventToSimEvent( EVT_MCP_COURSE_SELECTOR_L, COURSE_SELECTOR_L );
		mapClientEventToSimEvent( EVT_MCP_FD_SWITCH_L, FD_SWITCH_L );
		mapClientEventToSimEvent( EVT_MCP_AT_ARM_SWITCH, AT_ARM_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_N1_SWITCH, N1_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_SPEED_SWITCH, SPEED_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_CO_SWITCH, CO_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_SPEED_SELECTOR, SPEED_SELECTOR );
		mapClientEventToSimEvent( EVT_MCP_VNAV_SWITCH, VNAV_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_SPD_INTV_SWITCH, SPD_INTV_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_BANK_ANGLE_SELECTOR, BANK_ANGLE_SELECTOR );
		mapClientEventToSimEvent( EVT_MCP_HEADING_SELECTOR, HEADING_SELECTOR );
		mapClientEventToSimEvent( EVT_MCP_LVL_CHG_SWITCH, LVL_CHG_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_HDG_SEL_SWITCH, HDG_SEL_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_APP_SWITCH, APP_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_ALT_HOLD_SWITCH, ALT_HOLD_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_VS_SWITCH, VS_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_VOR_LOC_SWITCH, VOR_LOC_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_LNAV_SWITCH, LNAV_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_ALTITUDE_SELECTOR, ALTITUDE_SELECTOR );
		mapClientEventToSimEvent( EVT_MCP_VS_SELECTOR, VS_SELECTOR );
		mapClientEventToSimEvent( EVT_MCP_CMD_A_SWITCH, CMD_A_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_CMD_B_SWITCH, CMD_B_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_CWS_A_SWITCH, CWS_A_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_CWS_B_SWITCH, CWS_B_SWITCH );
		mapClientEventToSimEvent( EVT_MCP_DISENGAGE_BAR, DISENGAGE_BAR );
		mapClientEventToSimEvent( EVT_MCP_FD_SWITCH_R, FD_SWITCH_R );
		mapClientEventToSimEvent( EVT_MCP_COURSE_SELECTOR_R, COURSE_SELECTOR_R );
		mapClientEventToSimEvent( EVT_MCP_ALT_INTV_SWITCH, ALT_INTV_SWITCH );
	}
	catch( Exception & e )
	{
		throw;
	}
}


void 
MCPEntity::processNgxData( PMDG_NGX_Data *ngxData )
{
	using namespace Global::ValueId::MCP;

	compare( COURSE_L, ngxData->MCP_Course[ 0 ] );
	compare( COURSE_R, ngxData->MCP_Course[ 1 ] );
	compare( IAS_MACH, Utils::float2Raw( ngxData->MCP_IASMach ) );
	compare( IAS_OVERSPEED_FLASH, ngxData->MCP_IASOverspeedFlash );
	compare( IAS_UNDERSPEED_FLASH, ngxData->MCP_IASUnderspeedFlash );
	compare( HEADING, ngxData->MCP_Heading );
	compare( ALTITUDE, ngxData->MCP_Altitude );
	compare( VERT_SPEED, ngxData->MCP_VertSpeed );
	compare( VERT_SPEED_BLANK, ngxData->MCP_VertSpeedBlank );
	compare( FD_SW_L, ngxData->MCP_FDSw[ 0 ] );
	compare( FD_SW_R, ngxData->MCP_FDSw[ 1 ] );
	compare( AT_ARM_SW, ngxData->MCP_ATArmSw );
	compare( BANK_LIMIT_SEL, ngxData->MCP_BankLimitSel );
	compare( DISENGAGE_BAR, ngxData->MCP_DisengageBar );
	compare( ANNUN_FD_L, ngxData->MCP_annunFD[ 0 ] );
	compare( ANNUN_FD_R, ngxData->MCP_annunFD[ 1 ] );
	compare( ANNUN_AT_ARM, ngxData->MCP_annunATArm );
	compare( ANNUN_N1, ngxData->MCP_annunN1 );
	compare( ANNUN_SPEED, ngxData->MCP_annunSPEED );
	compare( ANNUN_VNAV, ngxData->MCP_annunVNAV );
	compare( ANNUN_LVL_CHG, ngxData->MCP_annunLVL_CHG );
	compare( ANNUN_HDG_SEL, ngxData->MCP_annunHDG_SEL );
	compare( ANNUN_LNAV, ngxData->MCP_annunLNAV );
	compare( ANNUN_VOR_LOC, ngxData->MCP_annunVOR_LOC );
	compare( ANNUN_APP, ngxData->MCP_annunAPP );
	compare( ANNUN_ALT_HOLD, ngxData->MCP_annunALT_HOLD );
	compare( ANNUN_VS, ngxData->MCP_annunVS );
	compare( ANNUN_CMD_A, ngxData->MCP_annunCMD_A );
	compare( ANNUN_CWS_A, ngxData->MCP_annunCWS_A );
	compare( ANNUN_CMD_B, ngxData->MCP_annunCMD_B );
	compare( ANNUN_CWS_B, ngxData->MCP_annunCWS_B );
}


void 
MCPEntity::compare( unsigned int valueId, uint32_t newValue )
{
	if( values[ valueId ] != newValue )
	{
		values[ valueId ].store( newValue );
		notifyOnEntityValueChangedListeners( valueId, newValue );
	}
}


void 
MCPEntity::transmitEvent( unsigned int eventId, uint32_t eventParameter )
{
	using namespace Global::EventParameter;

	switch( eventParameter )
	{
		case MOUSE_WHEEL_UP:
			SimConnect_TransmitClientEvent( simConnect, 0, eventId, MOUSE_FLAG_WHEEL_UP,
				SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY );
			break;

		case MOUSE_WHEEL_DOWN:
			SimConnect_TransmitClientEvent( simConnect, 0, eventId, MOUSE_FLAG_WHEEL_DOWN,
				SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY );
			break;

		case MOUSE_LEFT_CLICK:
			SimConnect_TransmitClientEvent( simConnect, 0, eventId, MOUSE_FLAG_LEFTSINGLE,
				SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY );
			SimConnect_TransmitClientEvent( simConnect, 0, eventId, MOUSE_FLAG_LEFTRELEASE,
				SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY );
			break;

		case MOUSE_RIGHT_CLICK:
			SimConnect_TransmitClientEvent( simConnect, 0, eventId, MOUSE_FLAG_RIGHTSINGLE,
				SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY );
			SimConnect_TransmitClientEvent( simConnect, 0, eventId, MOUSE_FLAG_RIGHTRELEASE,
				SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY );
			break;

		default:
			break;
	}
}


uint32_t 
MCPEntity::getSingleValue( unsigned int valueId )
{
	// Check if the id is valid.
	if( !Global::ValueId::MCP::isValid( valueId ) )
	{
		throw Exception( "invalid value-ID" );
	}

	return values[ valueId ].load();
}


void 
MCPEntity::getAllValues( uint32_t *values )
{
	// Loop through all value IDs and get the data.
	for( unsigned int valueId = 0; valueId < Global::ValueId::MCP::COUNT; valueId++ )
	{
		values[ valueId ] = this->values[ valueId ].load();
	}
}


size_t 
MCPEntity::getValueCount()
{
	return Global::ValueId::MCP::COUNT;
}