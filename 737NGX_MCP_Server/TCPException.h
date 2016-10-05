#pragma once


#include <exception>
using namespace std;


namespace TCP
{
    class TCPException : public exception
    {
    public:
        TCPException( const char *msg )
            : msg( msg ) {}

        const char *what() const throw() { return msg; }

    private:
        const char *msg;
    };
}
