#include "DataPack.h"
#include "MemoryCopy.h"

DataPack::DataPack( const std::string& _header, const void* _data, unsigned int _size )
: size(_size) 
{
	//Copies the value in memory to the array.
	MemoryCopy( header, _header.c_str(), static_cast<unsigned>(_header.length()) + 1 );
	//Copies the value in memory to the array.
	MemoryCopy( &data, _data, size );
}

DataPack::DataPack( const char* stream )
{
	const std::string _header = stream;

	//Move past the header to the size.
	const int* _size = reinterpret_cast<const int*>( stream + Constants::nameSize );
	
	//Move past the header and size to the data.
	const void* _data = _size + 1;

	//Copy the values over.
	*this = DataPack( _header, _data, *_size );
}
