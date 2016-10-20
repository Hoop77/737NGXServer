#pragma once


#include <functional>


namespace SimConnect
{
	// GoF Observer Pattern (Push Update Notification)
	class OnEntityValueChangedListener
	{
	public:
		OnEntityValueChangedListener( 
			std::function<void( unsigned int, unsigned int, uint32_t )> onEntityValueChanged )
			: onEntityValueChanged( onEntityValueChanged ) {}

		// unsigned int: value-ID
		// uint32_t: value
		std::function<void( unsigned int, uint32_t )>onEntityValueChanged;
	};
}