#include "MCPInterpreter.h"



MCPInterpreter::MCPInterpreter( MCPEntity *mcp )
    : mcp( mcp ) {}


MCPInterpreter::~MCPInterpreter() {}


void MCPInterpreter::interpretCommand( string cmd )
{
    if( cmd == "test" )
    {
        cout << "MCP test!" << endl;
    }
}
