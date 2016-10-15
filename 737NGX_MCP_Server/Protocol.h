#pragma once


#include <cstdint>


namespace Protocol
{
	class Exception : public std::exception
	{
	public:
		Exception( const char *msg )
			: msg( msg ) {}

		const char *what() const throw() { return msg; }

	private:
		const char *msg;
	};

    //-----------------------------------
    // Some definitions for the protocol.
    //-----------------------------------

    namespace Method
    {
        typedef uint8_t Type;

        constexpr Type GET = 0;
        constexpr Type SET = 1;

        inline bool isValid( Type method )
        {
            return method < 2 ? true : false;
        }
    }

    namespace BytePosition
    {
        typedef int Type;

        constexpr Type METHOD = 0;
        constexpr Type ENTITY_ID = 1;
        constexpr Type VALUE_ID_HIGH = 2;
        constexpr Type VALUE_ID_LOW = 3;
        constexpr Type VALUE_DATA = 4;
    };
}