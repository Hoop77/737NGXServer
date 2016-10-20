#pragma once

#include "SimConnectOnEntityValueChangedListener.h"
#include "SimConnect.h"
#include "Global.h"


#include <string>
#include <vector>
#include <windows.h>
#include <memory>
#include <functional>


namespace SimConnect
{
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

		void registerOnEntityValueChangedListener( std::shared_ptr<OnEntityValueChangedListener> listener );
		void deregisterOnEntityValueChangedListener( std::shared_ptr<OnEntityValueChangedListener> listener );

	protected:
		explicit Entity( const std::string & name );

		void notifyOnEntityValueChangedListeners( unsigned int valueId, uint32_t value );

		// helper function
		void mapClientEventToSimEvent( SIMCONNECT_CLIENT_EVENT_ID simConnectEventId, int valueId );

		HANDLE simConnect;
		std::vector<std::shared_ptr<OnEntityValueChangedListener>> valueListeners;
	};
}