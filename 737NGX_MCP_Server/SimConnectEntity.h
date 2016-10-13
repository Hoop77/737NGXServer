#pragma once

#include <iostream>
#include <string>

#include "SimConnect.h"
#include <windows.h>


class SimConnectEntity
{
public:
    explicit SimConnectEntity( const std::string & name );
    virtual ~SimConnectEntity();

    virtual void setup() = 0;
    virtual void dispatch( SIMCONNECT_RECV *data, DWORD size, void *context ) = 0;
    virtual void close() = 0;

    void obtainSimConnectHandle( HANDLE simConnect );
    std::string getName() const;

protected:
    const std::string name;
    HANDLE simConnect;
};


class SimConnectEntityException : std::exception
{
public:
    SimConnectEntityException( const char *msg )
        : msg( msg ) {}

    const char *what() const throw() { return msg; }

private:
    const char *msg;
};