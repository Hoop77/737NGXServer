#pragma once

#include <string>
using namespace std;

class CommandInterpreter
{
public:
    explicit CommandInterpreter() {}
    virtual ~CommandInterpreter() {}

    virtual void interpretCommand( string cmd ) = 0;
};