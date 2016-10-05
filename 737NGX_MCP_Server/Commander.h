#pragma once

#include <iostream>
#include <vector>
#include <thread>

#include "CommandInterpreter.h"

using namespace std;

class Commander
{
public:
	explicit Commander();
	virtual ~Commander();

	void startListening();
	void stopListening();
	bool isListening();
	void addInterpreter( CommandInterpreter *cmdInterpreter );

protected:
    static void listen( Commander *self );

	thread *listeningThread;
	bool listening;
	vector<CommandInterpreter *> interpreters;
};

