#pragma once

#include "TCPStream.h"

namespace TCP
{
    class TCPConnector
    {
    public:
        TCPStream *connect( const string server, uint16_t port );
    };
}
