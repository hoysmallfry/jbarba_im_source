#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <set>
#include <vector>

namespace Serializer
{
	// Serializes a set of type T into a vector of type T.
	template <typename T>
	std::vector<T> CrunchitizeMeCaptain( std::set<T> _set ) //Serializer, get it?
	{
		// Gets the size of the set.
		unsigned int size = (unsigned int)_set.size();

		// Allocates size+1 (ends vector with default value, strings = "", numbers = 0 )
		std::vector<T> _vector(size+1);
		// Iterates through the set.
		std::set<T>::iterator iter = _set.begin();
		
		// Copies the values to a vector in order.
		for( unsigned int index=0; index < size; index++, iter++  )
			_vector[index] = *iter;

		//Returns the vector.
		return _vector; 
	}

	// Takes a raw pointer(stream) and makes an array of a type T with size elements
	template <typename T>
	std::vector<T> SillyRabbit( const void* raw, unsigned int size )
	{
		// Allocates memory for members in vector.
		std::vector<T> _vector( size );
		//Starts position in stream at 0.
		unsigned int place = 0;
		//iterates for all members in size.
		for( unsigned int index=0; index < size; index++ )
		{
			//Pointer arithmetic with memory as chars. moves byte by byte.
			const char* newElementAddr = reinterpret_cast<const char*>(raw)+place;
			// Converts to specified type at address, adds it to the vector.
			_vector[index] = *( reinterpret_cast<const T*>(newElementAddr) );
			//Place increments by size of given type.
			place += sizeof(T);
		}

		//returns the vector.
		return _vector;
	}
}

#endif
