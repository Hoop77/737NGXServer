#include "Commander.h"


// static
static vector<CommandInterpreter *> interpreters;
static thread *listeningThread;


Commander::Commander()
    : listening( false ) {}


Commander::~Commander()
{
    stopListening();
    delete listeningThread;
}


void Commander::startListening()
{
    listening = true;
    // create a new thread for listening to commands
    listeningThread = new thread( Commander::listen, this );
}


void Commander::stopListening()
{
    listening = false;

    if( listeningThread )
        listeningThread->join();
}


bool Commander::isListening()
{
    return listening;
}


void Commander::addInterpreter( CommandInterpreter *cmdInterpreter )
{
    interpreters.push_back( cmdInterpreter );
}


void Commander::listen( Commander *self )
{
    string cmd = "";

    while( true )
    {
        // get command from console
        getline( cin, cmd );

        // stop if we shouldn't listen anymore
        if( !self->listening )
            break;

        // every interpreter will get the command
        for( auto interpreter : self->interpreters )
        {
            interpreter->interpretCommand( cmd );
        }
    }
}
