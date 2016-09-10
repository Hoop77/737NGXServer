#pragma once

#include <iostream>
#include <string>

#include "SimConnectEntity.h"
#include "SimConnect.h"
#include "PMDG_NGX_SDK.h"

using namespace std;

// Multi Control Panel
class MCP : public SimConnectEntity
{
public:
	explicit MCP( const string & name );
	virtual ~MCP();

    void setup();
	void dispatch( SIMCONNECT_RECV *data, DWORD size, void *context );
	void close();

protected:
	enum DataRequestId
	{
		DATA_REQUEST,
		CONTROL_REQUEST,
		AIR_PATH_REQUEST
	};

	void setupDataConnection();
	void setupControlConnection();

	PMDG_NGX_Control control;
};
