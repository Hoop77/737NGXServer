#pragma once

#include "CommandInterpreter.h"
#include "MCPEntity.h"

class MCPInterpreter : public CommandInterpreter
{
public:
	explicit MCPInterpreter( MCPEntity *mcp );
	virtual ~MCPInterpreter();

	void interpretCommand( const string cmd );

private:
	MCPEntity *mcp;
};

