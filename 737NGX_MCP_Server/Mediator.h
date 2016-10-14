#pragma once


#include "global.h"
#include "SimConnectEntity.h"


namespace EntityMediators
{
    class Mediator
    {
    public:
        virtual ~Mediator() {}

        virtual void setValueData( valueId_t valueId, value32_t valueData ) = 0;
        virtual value32_t getValueData( valueId_t valueId ) = 0;
        virtual bool setEntireData( const char *data, size_t size ) = 0;
        virtual bool getEntireData( char *data, size_t *size ) = 0;
    };
}