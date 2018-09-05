#include "MessagePack.h"
#include "MemoryCopy.h"

//Takes a stream and makes a MessagePack out of it.
MessagePack::MessagePack( const char* stream )
{
	//start reading 'to' at stream.
	const char* to = stream;
	//move past 'to' address to 'from'
	const char* from = stream + Constants::nameSize;
	//move past 'from' address to 'size'.
	const int* size = reinterpret_cast<const int*>(from + Constants::nameSize);
	//move past 'size' to 'message'.
	const char* message = reinterpret_cast<const char*>(size + 1);

	*this = MessagePack( to, from, message, *size );
}

//Takes a pointers and makes non-shallow copies of the data.
//The amount the data read for copy is limited by size.
MessagePack::MessagePack( const char* to, const char* from, const char* message, unsigned int size )
: Size( size )
{
	//copy value at address in 'to'
	MemoryCopy( &To, to, Constants::nameSize );
	//copy value at address in 'from'
	MemoryCopy( &From, from, Constants::nameSize );
	//copy value at address in 'message'
	MemoryCopy( &Message, message, Size );
}
