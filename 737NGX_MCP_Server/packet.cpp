#include "packet.h"

namespace Protocol
{
    void swap( Protocol::Packet & first, Protocol::Packet & second )
    {
        // enable ADL
        using std::swap;
        swap( first.data, second.data );
        swap( first.size, second.size );
    }
}