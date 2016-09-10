#include "SimConnectEntity.h"


SimConnectEntity::SimConnectEntity( const string & name )
	: name( name ) {}


SimConnectEntity::~SimConnectEntity() {}


void SimConnectEntity::obtainSimConnectHandle( HANDLE simConnect )
{
	this->simConnect = simConnect;
}


const string & SimConnectEntity::getName() const
{
	return name;
}