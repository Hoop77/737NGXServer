#pragma once


#include "SimConnectEntity.h"
#include "SimConnect.h"
#include "PMDG_NGX_SDK.h"


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
    };
}
