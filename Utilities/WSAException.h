#ifndef WSAEXCEPTION_H
#define WSAEXCEPTION_H

#include "SqueakyCleanWindows.h"
#include <Winsock2.h>
#include <exception>

class WSAException
	: public std::exception
{
private:
	// The string describing the type of error.
	const char* _error;
	// Gets the error message.

public:
	// Accepts an error value, saves it as a string.
	WSAException( const int& );
	// Returns the message.
	const char* what( void ) const;

	//Returns a string without instantiation of an object.
	static const char* getErrorCode( int errorCode );
};

#endif
