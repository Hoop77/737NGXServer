#pragma once

#include <string>
#include <vector>

#include "SimConnect.h"
#include <windows.h>
#include <memory>

#include "Global.h"


namespace SimConnect
{
    // GoF Observer Pattern (Push Update Notification)
    class EntityDataListener
    {
    public:
        virtual ~EntityDataListener() {}

        virtual void OnDataChanged( Global::ValueId::Type valueId, uint32_t valueData ) = 0;

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

        virtual void setValueData( Global::ValueId::Type valueId, uint32_t valueData ) = 0;
        virtual uint32_t getValueData( Global::ValueId::Type valueId ) = 0;
        virtual bool setEntireData( const char *data, size_t size ) = 0;
        virtual bool getEntireData( char *data, size_t *size ) = 0;

        void obtainSimConnectHandle( HANDLE simConnect );
        std::string getName() const;

        void registerDataListener( std::shared_ptr<EntityDataListener> listener );
        void deregisterDataListener( std::shared_ptr<EntityDataListener> listener );

    protected:
        explicit Entity( const std::string & name );

        void notifyDataListeners( Global::ValueId::Type valueId, uint32_t valueData );

        const std::string name;
        HANDLE simConnect;
        std::vector<std::shared_ptr<EntityDataListener>> dataListeners;
    };
}