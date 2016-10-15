#pragma once


#include "Protocol.h"
#include "Global.h"


#include <memory>
#include <cstdint>


namespace Protocol
{
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

        // public constructor (throwing)
        // Creates a packet from given data (for receiving).
		explicit Packet( char *data, size_t size );

        // public constructor (throwing)
        // Creates a packet with specified data (for sending).
		explicit Packet(
			Method::Type method,
			Global::EntityId::Type entityId,
			Global::ValueId::Type valueId,
			char *valueData,
			size_t valueDataSize 
		);

        // destructor
		virtual ~Packet();

        // copy constructor
		Packet( const Packet & other );

        // copy assignment operator
		Packet & operator=( Packet other );

        // move constructor
		Packet( Packet && other );

        // move assignment operator
		Packet & operator=( Packet && other );

		Method::Type getMethod() const;
		Global::EntityId::Type getEntityId() const;
		Global::ValueId::Type getValueId() const;
		const char *getValueData() const;

        const char *getData() const { return data; }
        size_t getSize() const { return size; }

    protected:
        // protected constructor (non-throwing)
		explicit Packet();

        char *data;
        size_t size;
    };
}