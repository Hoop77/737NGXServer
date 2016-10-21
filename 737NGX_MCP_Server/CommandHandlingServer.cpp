#include "CommandHandlingServer.h"
#include "CommandHandlingException.h"
#include <iostream>
#include "TCPAcceptor.h"
#include "TCPException.h"
#include "PacketFactory.h"
#include "Global.h"


using namespace CommandHandling;


Server::Server(
	const std::string & ip,
	uint16_t port,
	size_t connectionHandlerCount,
	std::shared_ptr<std::vector<std::unique_ptr<SimConnect::Entity>>> entities )
	: connectionHandlerCount( connectionHandlerCount )
	, entities( entities )
	, acceptor( ip, port )
	, running( false )
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
	try { stop(); }
	catch( ... ) {}
}


void 
Server::run()
{
	if( running )
		return;

	running = true;

	// Lets run the connection handlers.
	for( auto & connectionHandler : connectionHandlers )
	{
		connectionHandler->run();
	}

	// Permanently accept incoming TCP streams as client requests 
	// and add them to the queue to process them later.
	try
	{
		acceptor.start();

		while( running )
		{
			auto stream = acceptor.accept();
			streamQueue->enqueue( std::move( stream ) );
		}
	}
	catch( TCP::Exception & e )
	{
		// If the server is about to stop, we must stop the blocking "accept()" method.
		// To do this, someone must call "stop()" on the acceptor (in another thread) which will raise
		// an exception from the "accept()" method so we must ignore it when "running" is false!
		if( running )
			message( std::string( "TCP Error: " ).append( e.what() ) );
	}
}


void 
Server::stop()
{
	if( !running )
		return;

	try
	{
		// Stopping the connection handlers.
		for( auto & connectionHandler : connectionHandlers )
		{
			connectionHandler->stop();
		}

		// Stopping the blocking "accept()" method.
		acceptor.stop();
	}
	catch( ... )
	{
		message( "Cannot stop command handling server!\nIn order to close this application, use task manager!" );
	}
}


void 
Server::message( const std::string & msg )
{
	Global::println( msg );
}


void 
Server::handleEventPacket( Protocol::EventPacket *eventPacket )
{
	unsigned int entityId = eventPacket->getEntityId();
	unsigned int eventId = eventPacket->getEventId();
	uint32_t eventParameter = eventPacket->getEventParameter();

	// transmit the event to the entity
	try
	{
		entities->at( entityId )->transmitEvent( eventId, eventParameter );
	}
	catch( std::out_of_range & e )
	{
		message( "invalid entity-ID" );
	}
}


std::unique_ptr<Protocol::DataPacket> 
Server::handleRequestPacket( Protocol::RequestPacket *requestPacket )
{
	using namespace Protocol;

	// result
	std::unique_ptr<DataPacket> dataPacket( nullptr );

	// verify entity-ID
	unsigned int entityId = requestPacket->getEntityId();

	try
	{
		// get a pointer to the entity
		SimConnect::Entity *entity = entities->at( entityId ).get();

		int requestType = requestPacket->getRequestType();
		if( requestType == RequestPacket::REQUEST_TYPE_SINGLE_VALUE )
		{
			SingleValueRequestPacket *singleValueRequestPacket = static_cast<SingleValueRequestPacket *>( requestPacket );
			// get the value-ID from the packet
			unsigned int valueId = singleValueRequestPacket->getValueId();
			// get the corresponding value from the entity
			uint32_t value = entity->getSingleValue( valueId );
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
			entity->getAllValues( values.get() );
			// create data packet
			dataPacket = PacketFactory::createAllValuesDataPacket( entityId, values.get(), valueIdCount );
		}
		else
		{
			throw Exception( "invalid request type" );
		}
	}
	catch( std::out_of_range & e )
	{
		message( "invalid entity-ID" );
	}

	return dataPacket;
}

