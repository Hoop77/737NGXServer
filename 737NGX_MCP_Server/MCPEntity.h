#pragma once


#include "SimConnectEntity.h"
#include "SimConnect.h"
#include "PMDG_NGX_SDK.h"
#include "global.h"


#include <iostream>
#include <string>


namespace SimConnect
{
    // Multi Control Panel
    class MCPEntity : public Entity
    {
    public:
        explicit MCPEntity( const std::string & name );
        virtual ~MCPEntity();

        void setup();
        void dispatch( SIMCONNECT_RECV *data, DWORD size, void *context );
        void close();

    private:
        struct Data
        {
            value32_t       Course1;                // unsigned short
            value32_t       Course2;                // unsigned short
            value32_t       IASMach;                // float
            value32_t       IASBlank;               // bool
            value32_t       IASOverspeedFlash;      // bool
            value32_t       IASUnderspeedFlash;     // bool
            value32_t       Heading;                // unsigned short
            value32_t       Altitude;               // unsigned short
            value32_t       VertSpeed;              // short
            value32_t       VertSpeedBlank;         // bool

            value32_t       FDSw1;                  // bool
            value32_t       FDSw2;                  // bool
            value32_t       ATArmSw;                // bool
            value32_t       BankLimitSel;			// unsigned char
            value32_t       DisengageBar;           // bool

            value32_t       annunFD1;               // bool
            value32_t       annunFD2;               // bool
            value32_t		annunATArm;             // bool
            value32_t		annunN1;                // bool
            value32_t		annunSPEED;             // bool
            value32_t		annunVNAV;              // bool
            value32_t		annunLVL_CHG;           // bool
            value32_t		annunHDG_SEL;           // bool
            value32_t		annunLNAV;              // bool
            value32_t		annunVOR_LOC;           // bool
            value32_t		annunAPP;               // bool
            value32_t		annunALT_HOLD;          // bool
            value32_t		annunVS;                // bool
            value32_t		annunCMD_A;             // bool
            value32_t		annunCWS_A;             // bool
            value32_t		annunCMD_B;             // bool
            value32_t		annunCWS_B;             // bool
        };

        enum DataRequestId
        {
            DATA_REQUEST,
            CONTROL_REQUEST,
            AIR_PATH_REQUEST
        };

        enum EventId
        {
            EVENT_HEADING_SELECTOR,
            EVENT_KEYBOARD_A
        };

        enum InputId
        {
            INPUT
        };

        enum GroupId
        {
            GROUP_KEYBOARD
        };

        void setupDataConnection();
        void setupControlConnection();

        PMDG_NGX_Control control;
        Data data;
    };
}
