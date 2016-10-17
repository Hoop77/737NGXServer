#pragma once


#include "PacketFactory.h"
#include "Global.h"
#include <assert.h>


#include <memory>


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

		int getPacketType() const
		{
			return data[ BYTE_POS_PACKET_TYPE ];
		}

		unsigned int getEntityId() const
		{
			return data[ BYTE_POS_ENTITY_ID ];
		}

		char *getData() const
		{
			return data;
		}

		size_t getSize() const
		{
			return size;
		}

	protected:
		Packet() {}

		char *data;
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

		unsigned int getEventId()
		{
			unsigned int eventId = 
				data[ BYTE_POS_EVENT_ID_LOW_LOW ]
				+ (data[ BYTE_POS_EVENT_ID_LOW_HIGH ] << 8)
				+ (data[ BYTE_POS_EVENT_ID_HIGH_LOW ] << 16)
				+ (data[ BYTE_POS_EVENT_ID_HIGH_HIGH ] << 24);
			return eventId;
		}

		uint32_t getEventParameter()
		{
			unsigned int eventParameter =
				data[ BYTE_POS_EVENT_PARAMETER_LOW_LOW ]
				+ (data[ BYTE_POS_EVENT_PARAMETER_LOW_HIGH ] << 8)
				+ (data[ BYTE_POS_EVENT_PARAMETER_HIGH_LOW ] << 16)
				+ (data[ BYTE_POS_EVENT_PARAMETER_HIGH_HIGH ] << 24);
			return eventParameter;
		}

	protected:
		EventPacket()
			: Packet()
		{
			Packet::size = SIZE;
		}
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

		int getRequestType()
		{
			return data[ BYTE_POS_REQUEST_TYPE ];
		}

	protected:
		RequestPacket() : Packet() {}
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

		unsigned int getValueId()
		{
			unsigned int valueId =
				data[ BYTE_POS_VALUE_ID_LOW ]
				+ (data[ BYTE_POS_VALUE_ID_HIGH ] << 8);
			return valueId;
		}

	protected:
		SingleValueRequestPacket() 
			: RequestPacket()
		{
			size = SIZE;
		}
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
			: RequestPacket()
		{
			size = SIZE;
		}
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
		DataPacket() : Packet() {}
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
		SingleValueDataPacket( unsigned int entityId, unsigned int valueId, uint32_t value )
			: DataPacket()
		{
			size = SIZE;

			data = new char[ size ];
			// packet type
			data[ Packet::BYTE_POS_PACKET_TYPE ] = Packet::PACKET_TYPE_DATA;
			// entity id
			data[ Packet::BYTE_POS_ENTITY_ID ] = entityId;
			// request type
			data[ DataPacket::BYTE_POS_REQUEST_TYPE ] = RequestPacket::REQUEST_TYPE_SINGLE_VALUE;
			// value id
			data[ BYTE_POS_VALUE_ID_LOW ] = valueId & 0x000000ff;
			data[ BYTE_POS_VALUE_ID_HIGH ] = (valueId & 0x0000ff00) >> 8;
			// value
			data[ BYTE_POS_SINGLE_VALUE_LOW_LOW ] = value & 0x00000000;
			data[ BYTE_POS_SINGLE_VALUE_LOW_HIGH ] = (value & 0x0000ff00) >> 8;
			data[ BYTE_POS_SINGLE_VALUE_HIGH_LOW ] = (value & 0x00ff0000) >> 16;
			data[ BYTE_POS_SINGLE_VALUE_HIGH_HIGH ] = (value & 0xff000000) >> 24;
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
		AllValuesDataPacket( unsigned int entityId, uint32_t *values, size_t valueCount )
		{
			assert( valueCount <= MAX_VALUE_COUNT );

			data = new char[ MIN_SIZE + valueCount * 4 ];
			// packet type
			data[ Packet::BYTE_POS_PACKET_TYPE ] = Packet::PACKET_TYPE_DATA;
			// entity id
			data[ Packet::BYTE_POS_ENTITY_ID ] = entityId;
			// request type
			data[ DataPacket::BYTE_POS_REQUEST_TYPE ] = RequestPacket::REQUEST_TYPE_SINGLE_VALUE;
			// copy values
			std::memcpy( data, values, valueCount * 4 );
		}
	};
}