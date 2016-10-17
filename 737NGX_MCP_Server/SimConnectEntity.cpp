#include "SimConnectEntity.h"
#include "SimConnectException.h"
using namespace SimConnect;

#include <algorithm>


Entity::Entity( const std::string & name )
	: name( name ) {}


Entity::~Entity() {}


void Entity::obtainSimConnectHandle( HANDLE simConnect )
{
	this->simConnect = simConnect;
}


std::string Entity::getName() const
{
	return name;
}


void Entity::registerValueListener( std::shared_ptr<EntityValueListener> listener )
{
	valueListeners.push_back( listener );
}


void Entity::deregisterValueListener( std::shared_ptr<EntityValueListener> listener )
{
	auto pos = std::find_if( valueListeners.begin(), valueListeners.end(),
		[&]( std::shared_ptr<EntityValueListener> const & l )
	{
		return l.get() == listener.get();
	} );

	if( pos != valueListeners.end() )
		valueListeners.erase( pos );
}


void Entity::notifyValueListeners( unsigned int valueId, uint32_t value )
{
	for( auto & listener : valueListeners )
	{
		listener->OnValueChanged( valueId, value );
	}
}


void Entity::mapClientEventToSimEvent( SIMCONNECT_CLIENT_EVENT_ID simConnectEventId, int valueId )
{
	// The simconnect event id must be specified as a string starting with '#'.
	std::string id( std::string( "#" )
		.append( std::to_string( simConnectEventId ) ) );

	HRESULT result = SimConnect_MapClientEventToSimEvent( simConnect, valueId, id.c_str );
	if( result == E_FAIL ) 
		throw Exception( "SimConnect_MapClientEventToSimEvent" );
}