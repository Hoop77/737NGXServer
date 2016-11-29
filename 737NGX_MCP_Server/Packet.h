#pragma once


#include <memory>


namespace Protocol
{
	// ============================= CONSTANTS ============================= \\

	// fixed max size for every packet
	constexpr size_t PACKET_MAX_SIZE = 1024;

	// packet types
	namespace PacketType
	{
		constexpr int EVENT = 0;
		constexpr int REQUEST = 1;
		constexpr int DATA = 2;
	}

	// request types
	namespace RequestType
	{
		constexpr int SINGLE_VALUE = 0;
		constexpr int ALL_VALUES = 1;
	}

	namespace Response
	{
		constexpr int FAIL = 0;
		constexpr int OK = 1;
	}

	// ============================= PACKET TYPES ============================= \\


	// The most abstract packet type which is the root of all following packet types.
	class Packet
	{
	public:
		Packet( uint8_t packetType, uint8_t entityId ) 
			: packetType( packetType )
			, entityId( entityId ) {}

		virtual ~Packet() {}

		uint8_t getPacketType() { return packetType; }
		uint8_t getEntityId() { return entityId; }

	private:
		uint8_t packetType;
		uint8_t entityId;
	};


	// Packet from client transmitting an event to an entity.
	// Structure:
	// byte 	meaning
	// ----------------
	// 0		packet type
	// 1		entity id
	// 2-5		event-ID	
	// 6-9		event-parameter
	class EventPacket : public Packet
	{
	public:
		EventPacket( uint8_t entityId, uint32_t eventId, uint32_t eventParameter )
			: Packet( PacketType::EVENT, entityId )
			, eventId( eventId )
			, eventParameter( eventParameter ) {}

		uint32_t getEventId() { return eventId; }
		uint32_t getEventParameter() { return eventParameter; }

	private:
		uint32_t eventId;
		uint32_t eventParameter;
	};


	// Abstract packet type representing the idea that some client requests data from the server.
	class RequestPacket : public Packet
	{
	public:
		RequestPacket( uint8_t entityId, uint8_t requestType )
			: Packet( PacketType::REQUEST, entityId )
			, requestType( requestType ) {}

		virtual ~RequestPacket() {}

		uint8_t getRequestType() { return requestType; }

	private:
		uint8_t requestType;
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
	public:
		SingleValueRequestPacket( uint8_t entityId, uint16_t valueId )
			: RequestPacket( entityId, RequestType::SINGLE_VALUE )
			, valueId( valueId ) {}

		uint16_t getValueId() { return valueId; }

	private:
		uint16_t valueId;
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
	public:
		AllValuesRequestPacket( uint8_t entityId )
			: RequestPacket( entityId, RequestType::ALL_VALUES ) {}
	};


	// Abstract packet type representing the idea to send data from the server to the client.
	class DataPacket : public Packet
	{
	public:
		DataPacket( uint8_t entityId, uint8_t requestType )
			: Packet( PacketType::DATA, entityId )
			, requestType( requestType ) {}

		virtual ~DataPacket() {}

		uint8_t getRequestType() { return requestType; }

	private:
		uint8_t requestType;
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
	public:
		SingleValueDataPacket( uint8_t entityId, uint16_t valueId, uint32_t value )
			: DataPacket( entityId, RequestType::SINGLE_VALUE )
			, valueId( valueId )
			, value( value ) {}

		uint16_t getValueId() { return valueId; }
		uint32_t getValue() { return value; }

	private:
		uint16_t valueId;
		uint32_t value;
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
	public:
		AllValuesDataPacket( uint8_t entityId, std::unique_ptr<uint32_t> values, size_t valueCount )
			: DataPacket( entityId, RequestType::ALL_VALUES )
			, values( std::move( values ) )
			, valueCount( valueCount ) {}

		uint32_t *getValues() { return values.get(); }
		size_t getValueCount() { return valueCount; }

	private:
		std::unique_ptr<uint32_t> values;
		size_t valueCount;
	};
}