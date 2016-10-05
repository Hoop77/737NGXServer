#pragma once


#include "TCPException.h"


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ws2tcpip.h>


namespace TCP
{
    void init();
    void cleanup();
}