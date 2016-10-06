#pragma once


#include "CommandInterpreter.h"
#include "MCPEntity.h"


#include <string>
using namespace std;


class MCPInterpreter : public CommandInterpreter
{
public:
    explicit MCPInterpreter( MCPEntity *mcp );
    virtual ~MCPInterpreter();

    void interpretCommand( string cmd );

private:
    MCPEntity *mcp;
};

