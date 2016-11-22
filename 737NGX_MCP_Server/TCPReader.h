#include "TCPStream.h"


namespace TCP
{
    class Reader
    {
    public:
        Reader( Stream *stream );
        Reader( Stream *stream, size_t capacity );

		~Reader();

		Reader( Reader & other ) = delete;
		Reader & operator=( Reader other ) = delete;

		Reader( Reader && other );
		Reader & operator=( Reader && other );

        bool read( uint8_t *byte );
        bool read( uint8_t *buf, size_t buflen, size_t offset, size_t readlen );

		friend void swap( Reader & first, Reader & second )
		{
			using std::swap;

			swap( first.stream, second.stream );
			swap( first.readbuf, second.readbuf );
			swap( first.capacity, second.capacity );
			swap( first.validBytes, second.validBytes );
			swap( first.pos, second.pos );
		}

    private:
		Reader();

        Stream *stream;
        char *readbuf;
        size_t capacity;
        size_t validBytes;
        size_t pos;

        static constexpr size_t DEFAULT_CAPACITY = 1024;

        bool refill();
        void reset();
	};
}