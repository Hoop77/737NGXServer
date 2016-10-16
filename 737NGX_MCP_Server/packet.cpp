#include "packet.h"
#include "ProtocolException.h"


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


using namespace Protocol;


// protected constructor (non-throwing)
Packet::Packet() : data( nullptr ), size( 0 ) {}


// public constructor
Packet::Packet( char *data, size_t size )
	: data( data )
	, size( size )
{
	// A packet must hold at least 4 Byte.
	if( data == nullptr || size < MIN_SIZE || size > MAX_SIZE )
	{
		throw Exception( "invalid buffer" );
	}

	// Check if the method is well defined.
	Method::Type method = data[ BytePosition::METHOD ];
	if( !Method::isValid( method ) )
	{
		throw Exception( "invalid data" );
	}

	std::memcpy( this->data, data, size );
}


// public constructor
Packet::Packet(
	Method::Type method,
	Global::EntityId::Type entityId,
	Global::ValueId::Type valueId,
	char *valueData,
	size_t valueDataSize )
{
	if( valueData == nullptr ||
		valueDataSize + BytePosition::VALUE_DATA > MAX_SIZE )
	{
		throw Exception( "invalid value data buffer" );
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
Packet::~Packet()
{
	delete[] data;
}


// copy constructor
Packet::Packet( const Packet & other )
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
Packet &
Packet::operator=( Packet other )
{
	swap( *this, other );

	return *this;
}


// move constructor
Packet::Packet( Packet && other )
	: Packet()
{
	swap( *this, other );
}


// move assignment operator
Packet &
Packet::operator=( Packet && other )
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


Method::Type
Packet::getMethod() const
{
	return data[ BytePosition::METHOD ];
}


Global::EntityId::Type
Packet::getEntityId() const
{
	return data[ BytePosition::ENTITY_ID ];
}


Global::ValueId::Type
Packet::getValueId() const
{
	return (data[ BytePosition::VALUE_ID_HIGH ] << 8) + data[ BytePosition::VALUE_ID_LOW ];
}


const char *
Packet::getValueData() const
{
	return data + BytePosition::VALUE_DATA;
}


const char *
Packet::getData() const
{
	return data;
}


size_t
Packet::getSize() const
{
	return size;
}