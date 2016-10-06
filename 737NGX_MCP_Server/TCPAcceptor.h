#pragma once


#include "TCP.h"
#include "TCPStream.h"


#include <string>
using namespace std;


namespace TCP
{
    class Acceptor
    {
    public:
        explicit Acceptor( const string &address, uint16_t port );
        virtual ~Acceptor();

        // Copy protection since destructor closes the listening socket.
        Acceptor( const Acceptor &other ) = delete;
        Acceptor &operator=( const Acceptor &other ) = delete;

        void start();
        Stream *accept();

    private:
        SOCKET listenSocket;
        const string address;
        uint16_t port;
        bool listening;
    };
}