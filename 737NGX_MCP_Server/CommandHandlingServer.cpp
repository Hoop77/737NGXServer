#include "CommandHandlingServer.h"
#include "CommandHandlingException.h"
#include <iostream>
#include "TCPAcceptor.h"
#include "TCPException.h"
#include "SimConnectException.h"
#include "Global.h"


using namespace CommandHandling;


Server::Server(
	const std::string & hostname,
	uint16_t port,
	size_t connectionHandlerCount,
	std::shared_ptr<std::vector<std::unique_ptr<SimConnect::Entity>>> entities )
	: connectionHandlerCount( connectionHandlerCount )
	, entities( entities )
	, acceptor( hostname, port )
	, running( false )
{
	using Utils::WorkQueue;
	// initialize the streams queue
	streamQueue = std::make_unique<WorkQueue<std::unique_ptr<TCP::Stream>>>();

	// Creating the specified number of connection handlers (we don't let them run at this point).
	for( size_t i = 0; i < connectionHandlerCount; i++ )
	{
		connectionHandlers.push_back(
			std::make_unique<ConnectionHandler>( *this ) );
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
			message( "TCP Error: " + e.what() );
	}
}


void 
Server::stop()
{
	if( !running )
		return;

	running = false;

	try
	{
		// Stopping the blocking "accept()" method.
		acceptor.stop();

		// Stopping the connection handlers.
		for( auto & connectionHandler : connectionHandlers )
		{
			connectionHandler->stop();
		}
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


std::unique_ptr<Protocol::Packet> 
Server::handleRequestPacket( Protocol::RequestPacket *requestPacket )
{
	using namespace Protocol;

	// result
	std::unique_ptr<Packet> packet( nullptr );

	// verify entity-ID
	unsigned int entityId = requestPacket->getEntityId();

	try
	{
		// get a pointer to the entity
		SimConnect::Entity *entity = entities->at( entityId ).get();

		int requestType = requestPacket->getRequestType();
		if( requestType == RequestType::SINGLE_VALUE )
		{
			SingleValueRequestPacket *singleValueRequestPacket = static_cast<SingleValueRequestPacket *>(requestPacket);
			// get the value-ID from the packet
			unsigned int valueId = singleValueRequestPacket->getValueId();
			// get the corresponding value from the entity
			uint32_t value = entity->getSingleValue( valueId );
			// create data packet
			packet.reset( new SingleValueDataPacket( entityId, valueId, value ) );
		}
		else if( requestType == RequestType::ALL_VALUES )
		{
			AllValuesRequestPacket *allValuesRequestPacket = static_cast<AllValuesRequestPacket *>(requestPacket);
			// get the number of values of the entity
			size_t valueCount = entity->getValueCount();
			// create array of values
			std::unique_ptr<uint32_t> values( new uint32_t[ valueCount ] );
			// get the values from the entity
			entity->getAllValues( values.get() );
			// create data packet
			packet.reset( new AllValuesDataPacket( entityId, std::move( values ), valueCount ) );
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
	catch( SimConnect::Exception & e )
	{
		message( e.what() );
	}

	return packet;
}

