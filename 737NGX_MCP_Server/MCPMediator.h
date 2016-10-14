#pragma once


#include <mutex>


#include "Mediator.h"
#include "MCPEntity.h"


namespace EntityMediators
{
    class MCPMediator : public Mediator
    {
    public:
        MCPMediator( SimConnect::MCPEntity *mcpEntity );

        void setValueData( valueId_t valueId, value32_t valueData );
        value32_t getValueData( valueId_t valueId );
        bool setEntireData( const char *data, size_t size );
        bool getEntireData( char *data, size_t *size );

    private:
        SimConnect::MCPEntity *mcpEntity;
        std::mutex entityMutex;
    };
}
