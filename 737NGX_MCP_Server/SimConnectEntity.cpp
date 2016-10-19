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


void Entity::registerOnEntityValueChangedListener( std::shared_ptr<OnEntityValueChangedListener> listener )
{
	valueListeners.push_back( listener );
}


void Entity::deregisterOnEntityValueChangedListener( std::shared_ptr<OnEntityValueChangedListener> listener )
{
	auto pos = std::find_if( valueListeners.begin(), valueListeners.end(),
		[&]( std::shared_ptr<OnEntityValueChangedListener> const & l )
	{
		return l.get() == listener.get();
	} );

	if( pos != valueListeners.end() )
		valueListeners.erase( pos );
}


void Entity::notifyOnEntityValueChangedListeners( unsigned int valueId, uint32_t value )
{
	for( auto & listener : valueListeners )
	{
		listener->OnEntityValueChanged( valueId, value );
	}
}


void Entity::mapClientEventToSimEvent( SIMCONNECT_CLIENT_EVENT_ID simConnectEventId, int eventId )
{
	// The simconnect event id must be specified as a string starting with '#'.
	std::string id( std::string( "#" )
		.append( std::to_string( simConnectEventId ) ) );

	HRESULT result = SimConnect_MapClientEventToSimEvent( simConnect, eventId, id.c_str() );
	if( result == E_FAIL ) 
		throw Exception( "SimConnect_MapClientEventToSimEvent" );
}