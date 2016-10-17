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
	class EntityValueListener
	{
	public:
		virtual ~EntityValueListener() {}

		virtual void OnValueChanged( unsigned int valueId, uint32_t value ) = 0;

	protected:
		explicit EntityValueListener() {}
	};

	class Entity
	{
	public:
		virtual ~Entity();

		virtual void setup() = 0;
		virtual void dispatch( SIMCONNECT_RECV *data, DWORD size, void *context ) = 0;
		virtual void close() = 0;

		virtual void transmitEvent( unsigned int eventId, uint32_t eventParameter ) = 0;
		virtual uint32_t getSingleValue( unsigned int valueId ) = 0;
		virtual void getAllValues( uint32_t *values ) = 0;

		void obtainSimConnectHandle( HANDLE simConnect );
		std::string getName() const;

		void registerValueListener( std::shared_ptr<EntityValueListener> listener );
		void deregisterValueListener( std::shared_ptr<EntityValueListener> listener );

	protected:
		explicit Entity( const std::string & name );

		void notifyValueListeners( unsigned int valueId, uint32_t value );

		// helper function
		void mapClientEventToSimEvent( SIMCONNECT_CLIENT_EVENT_ID simConnectEventId, int valueId );

		const std::string name;
		HANDLE simConnect;
		std::vector<std::shared_ptr<EntityValueListener>> valueListeners;
	};
}