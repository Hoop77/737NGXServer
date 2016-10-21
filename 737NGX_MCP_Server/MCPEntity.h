#pragma once


#include "SimConnectEntity.h"
#include "SimConnect.h"
#include "PMDG_NGX_SDK.h"
#include "global.h"


#include <iostream>
#include <string>
#include <atomic>
#include <functional>


namespace SimConnect
{
	// Multi Control Panel
	class MCPEntity : public Entity
	{
	public:
		MCPEntity();

		static constexpr int DATA_REQUEST_ID = 0;
		
		void setup();
		void dispatch( SIMCONNECT_RECV *data, DWORD size, void *context );
		void close();

		void transmitEvent( unsigned int eventId, uint32_t eventParameter );
		uint32_t getSingleValue( unsigned int valueId );
		void getAllValues( uint32_t *values );

	private:
		void setupDataConnection();
		void setupEvents();
		void processNgxData( PMDG_NGX_Data *ngxData );
		void compare( unsigned int valueId, uint32_t newValue );

		// Atomic array storing our values.
		std::atomic<uint32_t> values[ Global::ValueId::MCP::COUNT ];
	};
}
