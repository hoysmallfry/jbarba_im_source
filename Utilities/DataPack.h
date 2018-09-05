#ifndef DATAPACK_H
#define DATAPACK_H

#include "Constants.h" //used to uniform the sizes.
#include <string>

//A structure for containing data.
struct DataPack
{
	//The header string. receivers handle data based on what the string is.
	char header[Constants::nameSize];
	//The size of the data.
	unsigned int size;
	//The data itself.
	char data[Constants::dataSize];

	//Assignment constructor. Takes three separate values and creates a datapack from them.
	DataPack( const std::string& _header, const void* _data, unsigned int _size );
	
	// Assignment constructor. A stream of chars is recast into a DataPack.
	DataPack( const char* stream );
};

#endif
