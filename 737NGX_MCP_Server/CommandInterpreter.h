#pragma once

#include <string>

using namespace std;

class CommandInterpreter
{
public:
    explicit CommandInterpreter() {}
    virtual ~CommandInterpreter() {}

    virtual void interpretCommand( const string & cmd ) = 0;
};