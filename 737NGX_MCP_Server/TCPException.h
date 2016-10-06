#pragma once


#include <exception>


namespace TCP
{
    class Exception : public std::exception
    {
    public:
        Exception( const char *msg )
            : msg( msg ) {}

        const char *what() const throw() { return msg; }

    private:
        const char *msg;
    };
}
