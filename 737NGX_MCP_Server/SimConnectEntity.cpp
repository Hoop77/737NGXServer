#include "SimConnectEntity.h"
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


void Entity::registerDataListener( std::shared_ptr<EntityDataListener> listener )
{
    dataListeners.push_back( listener );
}


void Entity::deregisterDataListener( std::shared_ptr<EntityDataListener> listener )
{
    auto pos = std::find_if( dataListeners.begin(), dataListeners.end(), 
        [&]( std::shared_ptr<EntityDataListener> const & l )
    {
        return l.get() == listener.get();
    } );

    if( pos != dataListeners.end() )
        dataListeners.erase( pos );
}


void Entity::notifyDataListeners( Global::ValueId::Type valueId, uint32_t valueData )
{
    for( auto & listener : dataListeners )
    {
        listener->OnDataChanged( valueId, valueData );
    }
}