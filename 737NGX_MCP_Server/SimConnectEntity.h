#pragma once

#include <iostream>
#include <string>

#include "SimConnect.h"
#include <windows.h>

using namespace std;

class SimConnectEntity
{
public:
    explicit SimConnectEntity( string name );
    virtual ~SimConnectEntity();

    virtual void setup() = 0;
    virtual void dispatch( SIMCONNECT_RECV *data, DWORD size, void *context ) = 0;
    virtual void close() = 0;

    void obtainSimConnectHandle( HANDLE simConnect );
    string getName() const;

protected:
    const string name;
    HANDLE simConnect;
};

