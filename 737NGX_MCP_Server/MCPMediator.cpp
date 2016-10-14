#include "MCPMediator.h"


using namespace EntityMediators;


MCPMediator::MCPMediator( SimConnect::MCPEntity *mcpEntity )
    : mcpEntity( mcpEntity )
{

}


void MCPMediator::setValueData( valueId_t valueId, value32_t valueData )
{

}


value32_t MCPMediator::getValueData( valueId_t valueId )
{
    value32_t value;
    value.b = true;
    return value;
}


bool MCPMediator::setEntireData( const char *data, size_t size )
{
    return true;
}


bool MCPMediator::getEntireData( char *data, size_t *size )
{
    return true;
}