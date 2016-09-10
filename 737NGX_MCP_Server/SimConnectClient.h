#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>
#include "SimConnect.h"
#include "SimConnectEntity.h"

using namespace std;

class SimConnectClient
{
public:
	static void addEntity( SimConnectEntity *entity );
	static void connect();

protected:
	static void CALLBACK dispatch( SIMCONNECT_RECV* data, DWORD size, void *context );

	static vector<SimConnectEntity *> entities;
	static bool quit;
};