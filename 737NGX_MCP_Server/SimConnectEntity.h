#pragma once

#include <string>
#include <vector>

#include "SimConnect.h"
#include <windows.h>
#include <memory>

#include "global.h"


namespace SimConnect
{
    // GoF Observer Pattern (Push Update Notification)
    class EntityDataListener
    {
    public:
        virtual ~EntityDataListener() {}

        virtual void OnDataChanged( valueId_t valueId, value32_t valueData ) = 0;

    protected:
        explicit EntityDataListener() {}
    };

    class Entity
    {
    public:
        virtual ~Entity();

        virtual void setup() = 0;
        virtual void dispatch( SIMCONNECT_RECV *data, DWORD size, void *context ) = 0;
        virtual void close() = 0;

        void obtainSimConnectHandle( HANDLE simConnect );
        std::string getName() const;

        void registerDataListener( std::shared_ptr<EntityDataListener> listener );
        void deregisterDataListener( std::shared_ptr<EntityDataListener> listener );

    protected:
        explicit Entity( const std::string & name );

        void notifyDataListeners( valueId_t valueId, value32_t valueData );

        const std::string name;
        HANDLE simConnect;
        std::vector<std::shared_ptr<EntityDataListener>> dataListeners;
    };
}