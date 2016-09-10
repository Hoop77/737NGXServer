#include <iostream>
#include <string>

#include "SimConnectClient.h"
#include "MCP.h"

using namespace std;

int main( int argc, char *argv[] )
{
	SimConnectEntity *mcp = new MCP( "Multi Control Panel Module" );

	SimConnectClient::addEntity( mcp );

	try
	{
		SimConnectClient::connect();
	}
	catch( const string & msg )
	{
		cout << msg << endl;
	}

	cin.get();

	return 0;
}





