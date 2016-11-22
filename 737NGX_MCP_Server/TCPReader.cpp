#include "TCPReader.h"
#include "TCPException.h"

using namespace TCP;

Reader::Reader( Stream *stream, size_t capacity )
    : stream( stream )
    , capacity( capacity )
{
    readbuf = new char[ capacity ];
}


Reader::Reader( Stream *stream )
    : Reader( stream, DEFAULT_CAPACITY ) 
{}


Reader::Reader() {}


Reader::~Reader()
{
	delete[] readbuf;
}


Reader::Reader( Reader && other )
	: Reader()
{
	swap( *this, other );
}


Reader & Reader::operator=( Reader && other )
{
	// steal
	stream = other.stream;
	readbuf = other.readbuf;
	capacity = other.capacity;
	validBytes = other.validBytes;
	pos = other.pos;
	
	// reset
	other.stream = nullptr;
	other.readbuf = nullptr;
	other.capacity = 0;
	other.validBytes = 0;
	other.pos = 0;

	return *this;
}


bool Reader::read( uint8_t *byte )
{
    if( pos < validBytes )
    {
        *byte = readbuf[ pos++ ];
        return true;
    }

    if( !refill() )
        return false;

    *byte = readbuf[ pos++ ];

    return true;
}


bool
Reader::read( uint8_t *buf, size_t buflen, size_t offset, size_t readlen )
{
    if( buf == nullptr || buflen - offset < readlen )
        throw Exception( "invalid buffer" );

	while( readlen > 0 )
	{
		size_t bytesRemaining = validBytes - pos;
		if( bytesRemaining == 0 )
		{
			if( !refill() )
				return false;

			bytesRemaining = validBytes;
		}

		size_t len;
		if( bytesRemaining >= readlen )
		{
			len = readlen;
		}
		else
		{
			len = bytesRemaining;
		}

		std::memcpy( buf + offset, readbuf + pos, len );
		offset += len;
		pos += len;
		readlen -= len;
	}

	return true;
}


bool 
Reader::refill()
{
    reset();

    try
    {
        validBytes = stream->read( readbuf, capacity );
    }
    catch( Exception & e )
    {
        throw;
    }
    
    if( validBytes == 0 )
        return false;

    return true;
}


void 
Reader::reset()
{
    validBytes = 0;
    pos = 0;
}