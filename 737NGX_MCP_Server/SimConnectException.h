#pragma once


#include <exception>


namespace SimConnect
{
	class Exception : std::exception
	{
	public:
		Exception( const char *msg )
			: msg( msg ) {}

		const char *what() const throw() { return msg; }

	private:
		const char *msg;
	};
}