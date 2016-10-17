#pragma once


#include "PMDG_NGX_SDK.h"


namespace Global
{
	namespace EntityId
	{
		constexpr unsigned int MCP = 0;

		constexpr unsigned int COUNT = 1;

		inline bool isValid( unsigned int entityId )
		{
			return entityId < COUNT ? true : false;
		}
	}

	// Value-IDs serve for getting values from an entity.
	namespace ValueId
	{
		// Multi Control Panel Value-IDs
		namespace MCP
		{
			constexpr unsigned int COURSE_L = 0;				// unsigned short
			constexpr unsigned int COURSE_R = 1;				// unsigned short
			constexpr unsigned int IAS_MACH = 2;				// float
			constexpr unsigned int IAS_BLANK = 3;				// bool
			constexpr unsigned int IAS_OVERSPEED_FLASH = 4;		// bool
			constexpr unsigned int IAS_UNDERSPEED_FLASH = 5;	// bool
			constexpr unsigned int HEADING = 6;					// unsigned short
			constexpr unsigned int ALTITUDE = 7;				// unsigned short
			constexpr unsigned int VERT_SPEED = 8;				// short
			constexpr unsigned int VERT_SPEED_BLANK = 9;		// bool
			constexpr unsigned int FD_SW_L = 10;				// bool
			constexpr unsigned int FD_SW_R = 11;				// bool
			constexpr unsigned int AT_ARM_SW = 12;				// bool
			constexpr unsigned int BANK_LIMIT_SEL = 13;			// unsigned char
			constexpr unsigned int DISENGAGE_BAR = 14;			// bool
			constexpr unsigned int ANNUN_FD_L = 15;				// bool
			constexpr unsigned int ANNUN_FD_R = 16;				// bool
			constexpr unsigned int ANNUN_AT_ARM = 17;			// bool
			constexpr unsigned int ANNUN_N1 = 18;				// bool
			constexpr unsigned int ANNUN_SPEED = 19;			// bool
			constexpr unsigned int ANNUN_VNAV = 20;				// bool
			constexpr unsigned int ANNUN_LVL_CHG = 21;			// bool
			constexpr unsigned int ANNUN_HDG_SEL = 22;			// bool
			constexpr unsigned int ANNUN_LNAV = 23;				// bool
			constexpr unsigned int ANNUN_VOR_LOC = 24;			// bool
			constexpr unsigned int ANNUN_APP = 25;				// bool
			constexpr unsigned int ANNUN_ALT_HOLD = 26;			// bool
			constexpr unsigned int ANNUN_VS = 27;				// bool
			constexpr unsigned int ANNUN_CMD_A = 28;			// bool
			constexpr unsigned int ANNUN_CWS_A = 29;			// bool
			constexpr unsigned int ANNUN_CMD_B = 30;			// bool
			constexpr unsigned int ANNUN_CWS_B = 31;			// bool

			constexpr unsigned int COUNT = 32;

			inline bool isValid( unsigned int valueId )
			{
				return valueId < COUNT ? true : false;
			}
		}
	}

	// Event-IDs for transmitting events to happen to simconnect
	namespace EventId
	{
		// Multi Control Panel Event-IDs
		namespace MCP
		{
			constexpr unsigned int COURSE_SELECTOR_L = EVT_MCP_COURSE_SELECTOR_L;
			constexpr unsigned int FD_SWITCH_L = EVT_MCP_FD_SWITCH_L;
			constexpr unsigned int AT_ARM_SWITCH = EVT_MCP_AT_ARM_SWITCH;
			constexpr unsigned int N1_SWITCH = EVT_MCP_N1_SWITCH;
			constexpr unsigned int SPEED_SWITCH = EVT_MCP_SPEED_SWITCH;
			constexpr unsigned int CO_SWITCH = EVT_MCP_CO_SWITCH;
			constexpr unsigned int SPEED_SELECTOR = EVT_MCP_SPEED_SELECTOR;
			constexpr unsigned int VNAV_SWITCH = EVT_MCP_VNAV_SWITCH;
			constexpr unsigned int SPD_INTV_SWITCH = EVT_MCP_SPD_INTV_SWITCH;
			constexpr unsigned int BANK_ANGLE_SELECTOR = EVT_MCP_BANK_ANGLE_SELECTOR;
			constexpr unsigned int HEADING_SELECTOR = EVT_MCP_HEADING_SELECTOR;
			constexpr unsigned int LVL_CHG_SWITCH = EVT_MCP_LVL_CHG_SWITCH;
			constexpr unsigned int HDG_SEL_SWITCH = EVT_MCP_HDG_SEL_SWITCH;
			constexpr unsigned int APP_SWITCH = EVT_MCP_APP_SWITCH;
			constexpr unsigned int ALT_HOLD_SWITCH = EVT_MCP_ALT_HOLD_SWITCH;
			constexpr unsigned int VS_SWITCH = EVT_MCP_VS_SWITCH;
			constexpr unsigned int VOR_LOC_SWITCH = EVT_MCP_VOR_LOC_SWITCH;
			constexpr unsigned int LNAV_SWITCH = EVT_MCP_LNAV_SWITCH;
			constexpr unsigned int ALTITUDE_SELECTOR = EVT_MCP_ALTITUDE_SELECTOR;
			constexpr unsigned int VS_SELECTOR = EVT_MCP_VS_SELECTOR;
			constexpr unsigned int CMD_A_SWITCH = EVT_MCP_CMD_A_SWITCH;
			constexpr unsigned int CMD_B_SWITCH = EVT_MCP_CMD_B_SWITCH;
			constexpr unsigned int CWS_A_SWITCH = EVT_MCP_CWS_A_SWITCH;
			constexpr unsigned int CWS_B_SWITCH = EVT_MCP_CWS_B_SWITCH;
			constexpr unsigned int DISENGAGE_BAR = EVT_MCP_DISENGAGE_BAR;
			constexpr unsigned int FD_SWITCH_R = EVT_MCP_FD_SWITCH_R;
			constexpr unsigned int COURSE_SELECTOR_R = EVT_MCP_COURSE_SELECTOR_R;
			constexpr unsigned int ALT_INTV_SWITCH = EVT_MCP_ALT_INTV_SWITCH;

			constexpr unsigned int COUNT = 28;

			inline bool isValid( unsigned int eventId )
			{
				return eventId < COUNT ? true : false;
			}
		}
	}

	inline unsigned int getValueIdCountFromEntityId( unsigned int entityId )
	{
		switch( entityId )
		{
		case EntityId::MCP:
			return ValueId::MCP::COUNT;
		default:
			return 0;
		}
	}
}