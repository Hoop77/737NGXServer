#pragma once


#include "PacketFactory.h"
#include "Global.h"
#include <assert.h>


#include <memory>


namespace Protocol
{
	class PacketFactory;

	class PacketException : public std::exception
	{
	public:
		PacketException( const char *msg )
			: msg( msg ) {}

		const char *what() const throw() { return msg; }

	private:
		const char *msg;
	};


	class Packet
	{
		friend class PacketFactory;

	public:
		// packet types
		static constexpr int PACKET_TYPE_EVENT = 0;
		static constexpr int PACKET_TYPE_REQUEST = 1;
		static constexpr int PACKET_TYPE_DATA = 2;

		// packet type (8 bit)
		static constexpr int BYTE_POS_PACKET_TYPE = 0;

		// entity-ID (8 bit)
		static constexpr int BYTE_POS_ENTITY_ID = 1;

		// packet type (1) + entity id (1)
		static constexpr size_t MIN_SIZE = 2;

		// fixed max size for every packet
		static constexpr size_t MAX_SIZE = 1024;

		virtual ~Packet()
		{
			delete[] data;
		}

		uint8_t getPacketType() const
		{
			return data[ BYTE_POS_PACKET_TYPE ];
		}

		uint8_t getEntityId() const
		{
			return data[ BYTE_POS_ENTITY_ID ];
		}

		uint8_t *getData() const
		{
			return data;
		}

		size_t getSize() const
		{
			return size;
		}

	protected:
		Packet( size_t size ) 
			: size( size )
		{
			data = new uint8_t[ size ];
		}

		uint8_t *data;
		size_t size;
	};


	// Packet from client transmitting an event to an entity.
	// Structure:
	// byte 	meaning
	// ----------------
	// 0		packet type
	// 1		entity id
	// 2-5		event-ID		
	class EventPacket : public Packet
	{
		friend class PacketFactory;

	public:
		// event-ID (32 bit)
		static constexpr int BYTE_POS_EVENT_ID_LOW_LOW = 2;
		static constexpr int BYTE_POS_EVENT_ID_LOW_HIGH = 3;
		static constexpr int BYTE_POS_EVENT_ID_HIGH_LOW = 4;
		static constexpr int BYTE_POS_EVENT_ID_HIGH_HIGH = 5;

		// event-parameter (32 bit)
		static constexpr int BYTE_POS_EVENT_PARAMETER_LOW_LOW = 6;
		static constexpr int BYTE_POS_EVENT_PARAMETER_LOW_HIGH = 7;
		static constexpr int BYTE_POS_EVENT_PARAMETER_HIGH_LOW = 8;
		static constexpr int BYTE_POS_EVENT_PARAMETER_HIGH_HIGH = 9;

		// packet min size + event-ID (4) + event-parameter
		static constexpr size_t SIZE = Packet::MIN_SIZE + 8;

		uint32_t getEventId()
		{
			uint32_t eventId = 0;
				std::memcpy( &eventId, data + BYTE_POS_EVENT_ID_LOW_LOW, 4 );
			return eventId;
		}

		uint32_t getEventParameter()
		{
			uint32_t eventParameter = 0;
			std::memcpy( &eventParameter, data + BYTE_POS_EVENT_PARAMETER_LOW_LOW, 4 );
			return eventParameter;
		}

	protected:
		EventPacket()
			: Packet( SIZE ) {}
	};


	class RequestPacket : public Packet
	{
		friend class PacketFactory;

	public:
		virtual ~RequestPacket() {}

		// request types
		static constexpr int REQUEST_TYPE_SINGLE_VALUE = 0;
		static constexpr int REQUEST_TYPE_ALL_VALUES = 1;

		// request type (8 bit)
		static constexpr int BYTE_POS_REQUEST_TYPE = 2;

		// packet min size + request type (1)
		static constexpr size_t MIN_SIZE = Packet::MIN_SIZE + 1;

		uint8_t getRequestType()
		{
			return data[ BYTE_POS_REQUEST_TYPE ];
		}

	protected:
		RequestPacket( size_t size ) 
			: Packet( size ) {}
	};


	// Packet from client requesting a single value from an entity.
	// Structure:
	// byte 	meaning
	// ----------------
	// 0		packet type
	// 1		entity id
	// 2		request type
	// 3-4		value-ID
	class SingleValueRequestPacket : public RequestPacket
	{
		friend class PacketFactory;

	public:
		// value-ID (16 bit)
		static constexpr int BYTE_POS_VALUE_ID_LOW = 3;
		static constexpr int BYTE_POS_VALUE_ID_HIGH = 4;

		// request packet min size + value-ID (2)
		static constexpr size_t SIZE = RequestPacket::MIN_SIZE + 2;

		uint16_t getValueId()
		{
			uint16_t valueId = 0;
			std::memcpy( &valueId, data + BYTE_POS_VALUE_ID_LOW, 2 );
			return valueId;
		}

	protected:
		SingleValueRequestPacket() 
			: RequestPacket( SIZE ) {}
	};


	// Packet from client requesting all values from an entity.
	// Structure:
	// byte 	meaning
	// ----------------
	// 0		packet type
	// 1		entity id
	// 2		request typ
	class AllValuesRequestPacket : public RequestPacket
	{
		friend class PacketFactory;

	public:
		// request packet min size (nothing more)
		static constexpr size_t SIZE = RequestPacket::MIN_SIZE;

	protected:
		AllValuesRequestPacket()
			: RequestPacket( SIZE ) {}
	};


	class DataPacket : public Packet
	{
		friend class PacketFactory;

	public:
		virtual ~DataPacket() {}

		// request type (8 bit)
		static constexpr int BYTE_POS_REQUEST_TYPE = 2;

		// packet min size + request type (1)
		static constexpr size_t MIN_SIZE = Packet::MIN_SIZE + 1;

	protected:
		DataPacket( size_t size ) 
			: Packet( size ) {}
	};


	// Packet from server sending a single value of an entity to a client.
	// Structure:
	// byte 	meaning
	// ----------------
	// 0		packet type
	// 1		entity id
	// 2		request typ
	// 3-4		value-ID
	// 5-8		value
	class SingleValueDataPacket : public DataPacket
	{
		friend PacketFactory;

	public:
		// value-ID (16 bit)
		static constexpr int BYTE_POS_VALUE_ID_LOW = 3;
		static constexpr int BYTE_POS_VALUE_ID_HIGH = 4;

		// value (32 bit)
		static constexpr int BYTE_POS_SINGLE_VALUE_LOW_LOW = 5;
		static constexpr int BYTE_POS_SINGLE_VALUE_LOW_HIGH = 6;
		static constexpr int BYTE_POS_SINGLE_VALUE_HIGH_LOW = 7;
		static constexpr int BYTE_POS_SINGLE_VALUE_HIGH_HIGH = 8;

		// data packet min size + value-ID (2) + value (4)
		static constexpr size_t SIZE = DataPacket::MIN_SIZE + 6;

	protected:
		// Data packet for single value request.
		SingleValueDataPacket( uint8_t entityId, uint16_t valueId, uint32_t value )
			: DataPacket( SIZE )
		{
			// packet type
			data[ Packet::BYTE_POS_PACKET_TYPE ] = Packet::PACKET_TYPE_DATA;
			// entity id
			data[ Packet::BYTE_POS_ENTITY_ID ] = entityId;
			// request type
			data[ DataPacket::BYTE_POS_REQUEST_TYPE ] = RequestPacket::REQUEST_TYPE_SINGLE_VALUE;
			// value id
			std::memcpy( data + BYTE_POS_VALUE_ID_LOW, &valueId, 2 );
			// value
			std::memcpy( data + BYTE_POS_SINGLE_VALUE_LOW_LOW, &value, 4 );
		}
	};


	// Packet from server sending all values of an entity to a client.
	// Structure:
	// byte 	meaning
	// ----------------
	// 0		packet type
	// 1		entity id
	// 2		request typ
	// ...		values (limited to the maximum packet size)
	class AllValuesDataPacket : public DataPacket
	{
		friend PacketFactory;

	public:
		// values start position (variable size)
		static constexpr int BYTE_POS_ALL_VALUES = 3;

		// data packet min size (nothing more)
		static constexpr size_t MIN_SIZE = DataPacket::MIN_SIZE;

		// max value count
		static constexpr int MAX_VALUE_COUNT = (MAX_SIZE - MIN_SIZE) / 4;

	protected:
		AllValuesDataPacket( uint8_t entityId, uint32_t *values, size_t valueCount )
			: DataPacket( MIN_SIZE + valueCount * 4 )
		{
			assert( valueCount <= MAX_VALUE_COUNT );

			// packet type
			data[ Packet::BYTE_POS_PACKET_TYPE ] = Packet::PACKET_TYPE_DATA;
			// entity id
			data[ Packet::BYTE_POS_ENTITY_ID ] = entityId;
			// request type
			data[ DataPacket::BYTE_POS_REQUEST_TYPE ] = RequestPacket::REQUEST_TYPE_SINGLE_VALUE;
			// copy values
			std::memcpy( data + BYTE_POS_ALL_VALUES, values, valueCount * 4 );
		}
	};
}