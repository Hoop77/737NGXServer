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
		explicit Client();
		virtual ~Client();

		void connect();
		void run();

		void addEntity( std::shared_ptr<Entity> entity );

	private:
		static void CALLBACK dispatch( SIMCONNECT_RECV* data, DWORD size, void *context );

		HANDLE simConnect;
		std::vector<std::shared_ptr<Entity>> entities;
		bool quit;
	};
}