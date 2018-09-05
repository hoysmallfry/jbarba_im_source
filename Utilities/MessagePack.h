#ifndef MESSAGEPACK_H
#define MESSAGEPACK_H

#include "Constants.h"

struct MessagePack
{
	// Who sent the message.
	char To[Constants::nameSize];
	// Which client its for.
	char From[Constants::nameSize];
	// Actual size of the string.
	int Size; 
	// The message sent.
	char Message[Constants::messageSize];

	// Takes a stream and makes a MessagePack out of it.
	MessagePack( const char* stream );

	// Takes a pointers and makes non-shallow copies of the data.
	// The amount the data read for copy is limited by size.
	MessagePack( const char* to, const char* from, const char* message, unsigned int size );
};

#endif
