#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <exception>
#include <memory>

#include <Windows.h>
#include "SimConnect.h"
#include "SimConnectEntity.h"
#include "SimConnectException.h"


namespace SimConnect
{
	class Client
	{
	public:
		Client( std::shared_ptr<std::vector<std::unique_ptr<Entity>>> entities );

		void connect();
		void run();

	private:
		static void CALLBACK dispatch( SIMCONNECT_RECV* data, DWORD size, void *context );

		HANDLE simConnect;
		std::shared_ptr<std::vector<std::unique_ptr<Entity>>> entities;
		bool quit;
	};
}