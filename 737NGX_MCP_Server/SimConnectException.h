#pragma once


#include <string>


namespace SimConnect
{
	class Exception
	{
	public:
		Exception( std::string msg )
			: msg( msg ) {}

		std::string what() const { return msg; }

	private:
		std::string msg;
	};
}