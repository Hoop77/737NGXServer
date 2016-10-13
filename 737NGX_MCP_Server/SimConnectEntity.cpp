#include "SimConnectEntity.h"


SimConnectEntity::SimConnectEntity( const std::string & name )
    : name( name ) {}


SimConnectEntity::~SimConnectEntity() {}


void SimConnectEntity::obtainSimConnectHandle( HANDLE simConnect )
{
    this->simConnect = simConnect;
}


std::string SimConnectEntity::getName() const
{
    return name;
}