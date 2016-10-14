#pragma once


#include <memory>
#include <stdint.h>


namespace Protocol
{
    class PacketException : public std::exception
    {
    public:
        PacketException( const char *msg )
            : msg( msg ) {}

        const char *what() const throw() { return msg; }

    private:
        const char *msg;
    };


    // Packet overview:
    //
    // Byte		Meaining
    // -----------------
    // 0		Method
    // 1		Entity-ID
    // 2-3		Value-ID
    // ...		Value-Data
    //
    // Big endiannes is used to represent integer values.

    // Base class for packets of the transferring protocol send over TCP.
    // Note: This is a resource class that makes use of the copy and swap idiom.
    class Packet
    {
        friend void swap( Packet & first, Packet & second );

    public:
        // maximum Packet size allowed:
        static const size_t MAX_SIZE = 1024;
        // Method, Entity-ID and Value-ID must be specified.
        static const size_t MIN_SIZE = 4;

        enum BytePosition
        {
            METHOD,
            ENTITY_ID,
            VALUE_ID_HIGH,
            VALUE_ID_LOW,
            VALUE_DATA
        };

        enum Method
        {
            SET,
            GET
        };

        // public constructor (throwing)
        // Creates a packet from given data (for receiving).
        explicit Packet( char *data, size_t size )
            : data( data )
            , size( size )
        {
            // A packet must hold at least 4 Byte.
            if( data == nullptr || size < MIN_SIZE || size > MAX_SIZE )
            {
                throw PacketException( "invalid buffer" );
            }

            // Check if the method is well defined.
            Method method = (Method) data[ BytePosition::METHOD ];
            if( method != Method::SET && method != Method::GET )
            {
                throw PacketException( "invalid data" );
            }

            std::memcpy( this->data, data, size );
        }

        // public constructor (throwing)
        // Creates a packet with specified data (for sending).
        explicit Packet(
            Method method,
            uint8_t entityId,
            uint16_t valueId,
            char *valueData,
            size_t valueDataSize )
        {
            if( valueData == nullptr || valueDataSize + BytePosition::VALUE_DATA > MAX_SIZE )
            {
                throw PacketException( "invalid value data buffer" );
            }

            size = BytePosition::VALUE_DATA + valueDataSize;
            data = new char[ size ];
            data[ BytePosition::METHOD ] = method;
            data[ BytePosition::ENTITY_ID ] = entityId;
            data[ BytePosition::VALUE_ID_HIGH ] = (valueId & 0xff00) >> 8;
            data[ BytePosition::VALUE_ID_LOW ] = valueId & 0x00ff;
            std::memcpy( data + BytePosition::VALUE_DATA, valueData, valueDataSize );
        }

        // destructor
        virtual ~Packet()
        {
            delete[] data;
        }

        // copy constructor
        Packet( const Packet & other )
            : size( other.size )
        {
            if( size == 0 )
            {
                data = nullptr;
            }
            else
            {
                data = new char[ size ];
                std::memcpy( data, other.data, size );
            }
        }

        // copy assignment operator
        Packet & operator=( Packet other )
        {
            swap( *this, other );

            return *this;
        }

        // move constructor
        Packet( Packet && other )
            : Packet()
        {
            swap( *this, other );
        }

        // move assignment operator
        Packet & operator=( Packet && other )
        {
            if( this != &other )
            {
                delete[] data;

                size = other.size;
                data = other.data;

                other.size = 0;
                other.data = nullptr;
            }

            return *this;
        }

        Method getMethod()
        {
            return (Method) data[ BytePosition::METHOD ];
        }

        uint8_t getEntityId()
        {
            return data[ BytePosition::ENTITY_ID ];
        }

        uint16_t getValueId()
        {
            return (data[ BytePosition::VALUE_ID_HIGH ] << 8) + data[ BytePosition::VALUE_ID_LOW ];
        }

        const char *getValueData()
        {
            return data + BytePosition::VALUE_DATA;
        }

        const char *getData() const { return data; }
        size_t getSize() const { return size; }

    protected:
        // protected constructor (non-throwing)
        explicit Packet() : data( nullptr ), size( 0 ) {}

        char *data;
        size_t size;
    };
}