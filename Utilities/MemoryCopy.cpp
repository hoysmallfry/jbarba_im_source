#include "MemoryCopy.h"

void MemoryCopy( void* target, const void* source, unsigned int size )
{
	// Iterates through size times.
	for( unsigned int place = 0; place < size; place++ )
	{
		//use pointer arithmetic on char, get the value at the address. copy it over.
		*( reinterpret_cast< char*>(target) + place ) = *( reinterpret_cast<const char*>(source) + place );
	}
}
