#pragma once


#include <cstdint>
#include <memory>


namespace Utils
{
	inline uint32_t float2Raw( float f )
	{
		uint32_t raw;
		std::memcpy( &raw, &f, sizeof( float ) );
		return raw;
	}
}