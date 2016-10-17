#pragma once


namespace Global
{
	namespace EntityId
	{
		constexpr unsigned int MCP = 0;
	}

	// Value-IDs serve for interfacing the data of entities.
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
			constexpr unsigned int ANNUN_CSW_A = 29;			// bool
			constexpr unsigned int ANNUN_CMD_B = 30;			// bool
			constexpr unsigned int ANNUN_CSW_B = 31;			// bool

			constexpr unsigned int COUNT = 32;

			inline bool isValid( unsigned int valueId )
			{
				return valueId < COUNT ? true : false;
			}
		}
	}
}