#pragma once


#include "TCPStream.h"


#include <string>
using namespace std;


namespace TCP
{
    class Connector
    {
    public:
        Stream *connect( const string & server, uint16_t port );
    };
}
