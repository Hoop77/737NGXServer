#pragma once


#include "TCPStream.h"


#include <string>
#include <memory>


namespace TCP
{
    class Connector
    {
    public:
        std::unique_ptr<Stream> connect( const std::string & server, uint16_t port );
    };
}
