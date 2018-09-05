#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <map>
#include <string>

#include "Socket.h"

class SocketManager
{
private:
	//Creates a map to hold all the sockets under names.
	typedef std::pair<std::string, Socket*> SocketPair;
	typedef std::map<std::string, Socket*> SocketMap;
	typedef SocketMap::iterator SocketMapIterator;

	SocketMap _socketMap;

	// Destroys the pointer in the map.
	void destroyObject( Socket** socket );

public:
	// Default constructor.
	SocketManager( void );
	// Destructor.
	~SocketManager( void );

	// Creates a empty Socket and adds it to the map under the name.
	void add( const std::string& name );
	// Creates a socket, copies values over from Socket passed, adds it to map under tha name.
	void add( const std::string& name, const Socket& socket );
	// Creates a socket, copies passed value to SOCKET member, adds it to the map under the name. 
	void add( const std::string& name, const SOCKET& socket );
	
	// Retrieves the pointer in the map under the name.
	Socket* get( const std::string& name );
	
	// Deallocates memory being pointed to, removes the element under name.
	void remove( const std::string& name );
	// Deallocates all memory, clears the map.
	void removeAll( void );

	// Determines if a member under given key name exists.
	bool isMember( const std::string& name );
};

#endif
