#pragma once


namespace Global
{
	namespace EntityId
	{
		typedef uint8_t Type;

		constexpr Type MCP = 0;
	}

	// Value-IDs serve for interfacing the data of entities.
	namespace ValueId
	{
		typedef uint16_t Type;

		// Multi Control Panel Value-IDs
		namespace MCP
		{
			// values
			constexpr Type COURSE_1 = 0;				// unsigned short
			constexpr Type COURSE_2 = 1;				// unsigned short
			constexpr Type IAS_MACH = 2;				// float
			constexpr Type IAS_BLANK = 3;				// bool
			constexpr Type IAS_OVERSPEED_FLASH = 4;		// bool
			constexpr Type IAS_UNDERSPEED_FLASH = 5;	// bool
			constexpr Type HEADING = 6;					// unsigned short
			constexpr Type ALTITUDE = 7;				// unsigned short
			constexpr Type VERT_SPEED = 8;				// short
			constexpr Type VERT_SPEED_BLANK = 9;		// bool

			constexpr Type FD_SW_1 = 10;				// bool
			constexpr Type FD_SW_2 = 11;				// bool
			constexpr Type AT_ARM_SW = 12;				// bool
			constexpr Type BANK_LIMIT_SEL = 13;			// unsigned char
			constexpr Type DISENGAGE_BAR = 14;			// bool

			constexpr Type ANNUN_FD_1 = 15;				// bool
			constexpr Type ANNUN_FD_2 = 16;				// bool
			constexpr Type ANNUN_AT_ARM = 17;			// bool
			constexpr Type ANNUN_N1 = 18;				// bool
			constexpr Type ANNUN_SPEED = 19;			// bool
			constexpr Type ANNUN_VNAV = 20;				// bool
			constexpr Type ANNUN_LVL_CHG = 21;			// bool
			constexpr Type ANNUN_HDG_SEL = 22;			// bool
			constexpr Type ANNUN_LNAV = 23;				// bool
			constexpr Type ANNUN_VOR_LOC = 24;			// bool
			constexpr Type ANNUN_APP = 25;				// bool
			constexpr Type ANNUN_ALT_HOLD = 26;			// bool
			constexpr Type ANNUN_VS = 27;				// bool
			constexpr Type ANNUN_CMD_A = 28;			// bool
			constexpr Type ANNUN_CSW_A = 29;			// bool
			constexpr Type ANNUN_CMD_B = 30;			// bool
			constexpr Type ANNUN_CSW_B = 31;			// bool

			// number of value-IDs
			constexpr Type VALUE_COUNT = 32;

			// No real "value" but used to get/set the entire data at once.
			constexpr Type ENTIRE = 33;

			// total number of IDs
			constexpr Type TOTAL_COUNT = 34;

			inline bool isValid( Type valueId )
			{
				return valueId < TOTAL_COUNT ? true : false;
			}
		}
	}
}