#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include <Windows.h>
#include "SimConnect.h"
#include "SimConnectEntity.h"

using namespace std;

class SimConnectClient
{
public:
    explicit SimConnectClient();
    virtual ~SimConnectClient();

    void connect();
    void run();

    void addEntity( SimConnectEntity *entity );

protected:
    static void CALLBACK dispatch( SIMCONNECT_RECV* data, DWORD size, void *context );

    HANDLE simConnect;
    vector<SimConnectEntity *> entities;
    bool quit;
};