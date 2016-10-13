#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <exception>

#include <Windows.h>
#include "SimConnect.h"
#include "SimConnectEntity.h"


class SimConnectClient
{
public:
    explicit SimConnectClient();
    virtual ~SimConnectClient();

    void connect();
    void run();

    void addEntity( SimConnectEntity *entity );

private:
    static void CALLBACK dispatch( SIMCONNECT_RECV* data, DWORD size, void *context );

    HANDLE simConnect;
    std::vector<SimConnectEntity *> entities;
    bool quit;
};


class SimConnectClientException : std::exception
{
public:
    SimConnectClientException( const char *msg )
        : msg( msg ) {}

    const char *what() const throw() { return msg; }

private:
    const char *msg;
};