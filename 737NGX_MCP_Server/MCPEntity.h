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
		explicit MCPEntity( const std::string & name );

		void setup();
		void dispatch( SIMCONNECT_RECV *data, DWORD size, void *context );
		void close();

		void setValueData( Global::ValueId::Type valueId, uint32_t valueData );
		uint32_t getValueData( Global::ValueId::Type valueId );
		void setEntireData( const uint32_t *valueData );
		void getEntireData( uint32_t *valueData );

	private:
		enum DataRequestId
		{
			DATA_REQUEST,
			AIR_PATH_REQUEST
		};

		void setupDataConnection();
		void setupEvents();

		// Atomic array storing our value data.
		std::atomic<uint32_t> values[ Global::ValueId::MCP::VALUE_COUNT ];
	};
}
