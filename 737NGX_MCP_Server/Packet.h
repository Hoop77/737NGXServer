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
	struct Packet
	{
		uint8_t packetType;
		uint8_t entityId;

		virtual ~Packet() {}
	};


	// Packet from client transmitting an event to an entity.
	// Structure:
	// byte 	meaning
	// ----------------
	// 0		packet type
	// 1		entity id
	// 2-5		event-ID	
	// 6-9		event-parameter
	struct EventPacket : Packet
	{
		uint32_t eventId;
		uint32_t eventParameter;
	};


	// Abstract packet type representing the idea that some client requests data from the server.
	struct RequestPacket : Packet
	{
		uint8_t requestType;

		virtual ~RequestPacket() {}
	};


	// Packet from client requesting a single value from an entity.
	// Structure:
	// byte 	meaning
	// ----------------
	// 0		packet type
	// 1		entity id
	// 2		request type
	// 3-4		value-ID
	struct SingleValueRequestPacket : RequestPacket
	{
		uint16_t valueId;
	};


	// Packet from client requesting all values from an entity.
	// Structure:
	// byte 	meaning
	// ----------------
	// 0		packet type
	// 1		entity id
	// 2		request typ
	struct AllValuesRequestPacket : RequestPacket
	{
	};


	// Abstract packet type representing the idea to send data from the server to the client.
	struct DataPacket : Packet
	{
		uint8_t requestType;

		virtual ~DataPacket() {}
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
	struct SingleValueDataPacket : DataPacket
	{
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
	struct AllValuesDataPacket : DataPacket
	{
		std::unique_ptr<uint32_t> values;
		size_t valueCount;
	};
}