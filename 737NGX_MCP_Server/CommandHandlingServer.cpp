#include "CommandHandlingServer.h"
#include "CommandHandlingException.h"
#include <iostream>
#include "TCPAcceptor.h"
#include "TCPException.h"
#include "PacketFactory.h"


using namespace CommandHandling;


Server::Server( 
	const std::string & ip, 
	uint16_t port, 
	size_t connectionHandlerCount, 
	std::vector<std::unique_ptr<SimConnect::Entity>> & entities )
	: ip( ip )
	, port( port )
	, connectionHandlerCount( connectionHandlerCount )
	, entities( entities )
{
	using Utils::WorkQueue;
	using std::unique_ptr;
	// initialize the streams queue
	streamQueue = unique_ptr<WorkQueue<unique_ptr<TCP::Stream>>>(
		new WorkQueue<unique_ptr<TCP::Stream>>() );

	// Creating the specified number of connection handlers (we don't let them run at this point).
	for( size_t i = 0; i < connectionHandlerCount; i++ )
	{
		connectionHandlers.push_back(
			unique_ptr<ConnectionHandler>( new ConnectionHandler( *this ) ) );
	}
}


Server::~Server()
{
	stop();
}


void 
Server::run()
{
	// Lets run the connection handlers.
	for( auto & connectionHandler : connectionHandlers )
	{
		connectionHandler->run();
	}

	try
	{
		// Accept incoming TCP connections and add them to the queue.
		TCP::Acceptor acceptor( ip, port );
		acceptor.start();

		while( 1 )
		{
			auto stream = acceptor.accept();
			streamQueue->enqueue( std::move( stream ) );
		}
	}
	catch( TCP::Exception & e )
	{
		message( std::string( "TCP Error: " ).append( e.what() ) );
	}
}


void 
Server::stop()
{
	// Stopping the connection handlers.
	for( auto & connectionHandler : connectionHandlers )
	{
		connectionHandler->stop();
	}
}


void 
Server::message( const std::string & msg )
{
	std::lock_guard<std::mutex> lock( printMutex );
	std::cout << msg << std::endl;
}


void 
Server::handleEventPacket( Protocol::EventPacket *eventPacket )
{
	// verify entity-ID
	unsigned int entityId = eventPacket->getEntityId();
	if( !Global::EntityId::isValid( entityId ) )
	{
		message( "Invalid entity-ID!" );
		return;
	}

	unsigned int eventId = eventPacket->getEventId();
	uint32_t eventParameter = eventPacket->getEventParameter();

	// transmit the event to the entity
	entities[ entityId ]->transmitEvent( eventId, eventParameter );
}


std::unique_ptr<Protocol::DataPacket> 
Server::handleRequestPacket( Protocol::RequestPacket *requestPacket )
{
	using namespace Protocol;

	// verify entity-ID
	unsigned int entityId = requestPacket->getEntityId();
	if( !Global::EntityId::isValid( entityId ) )
	{
		throw Exception( "invalid entity-ID" );
	}

	std::unique_ptr<DataPacket> dataPacket( nullptr );

	int requestType = requestPacket->getRequestType();
	if( requestType == RequestPacket::REQUEST_TYPE_SINGLE_VALUE )
	{
		SingleValueRequestPacket *singleValueRequestPacket = static_cast<SingleValueRequestPacket *>( requestPacket );
		// get the value-ID from the packet
		unsigned int valueId = singleValueRequestPacket->getValueId();
		// get the corresponding value from the entity
		uint32_t value = entities[ entityId ]->getSingleValue( valueId );
		// create data packet
		dataPacket = PacketFactory::createSingleValueDataPacket( entityId, valueId, value );
	}
	else if( requestType == RequestPacket::REQUEST_TYPE_ALL_VALUES )
	{
		AllValuesRequestPacket *allValuesRequestPacket = static_cast<AllValuesRequestPacket *>(requestPacket);
		// get the number of values of the entity
		unsigned int valueIdCount = Global::getValueIdCountFromEntityId( entityId );
		// create array of values
		std::unique_ptr<uint32_t> values( new uint32_t[ valueIdCount ] );
		// get the values from the entity
		entities[ entityId ]->getAllValues( values.get() );
		// create data packet
		dataPacket = PacketFactory::createAllValuesDataPacket( entityId, values.get(), valueIdCount );
	}
	else
	{
		throw Exception( "invalid request type" );
	}

	return dataPacket;
}

