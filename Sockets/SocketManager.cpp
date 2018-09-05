#include "SocketManager.h"

//Default constructor.
SocketManager::SocketManager( void )
{}

SocketManager::~SocketManager( void )
{
	//Removes all the members before deleting.
	removeAll();
}

void SocketManager::add( const std::string& name )
{
	// If not part of map, allocates memory for a member.
	SocketMapIterator found = _socketMap.find(name);
	if( found == _socketMap.end() )
		_socketMap[name] = new Socket();
	// Otherwise, throws an error.
	else
		throw "There is already a member with that name.";
}

void SocketManager::add( const std::string& name, const Socket& socket )
{
	// If not part of map, allocates memory for a member.
	SocketMapIterator found = _socketMap.find(name);
	if( found == _socketMap.end() )
	{
		_socketMap[name] = new Socket( socket );
	}
	else
		throw "There is already a member with that name.";
}

void SocketManager::add( const std::string& name, const SOCKET& socket )
{
	// If not part of map, allocates memory for a member.
	SocketMapIterator found = _socketMap.find(name);
	if( found == _socketMap.end() )
		_socketMap[name] = new Socket( socket );
	// Otherwise, throws an error.
	else
		throw "There is already a member with that name.";
}

Socket* SocketManager::get( const std::string& name )
{
	//If member under name exists, return object pointer.
	SocketMapIterator found = _socketMap.find(name);
	if( found != _socketMap.end() )
		return found->second;
	return 0;
}

void SocketManager::remove( const std::string& name )
{
	//If member under name exists, destroy object.
	SocketMapIterator found = _socketMap.find(name);
	if( found != _socketMap.end() )
			destroyObject(&found->second);
	//erase members
	_socketMap.erase( name );
}

void SocketManager::removeAll( void )
{
	//Iterate through the map.
	SocketMapIterator iter = _socketMap.begin();
	while( iter != _socketMap.end() )
	{
		//Delete pointer
		destroyObject(&iter->second);
		++iter;
	}
	//clear members
	_socketMap.clear();
}

bool SocketManager::isMember( const std::string& name )
{
	// Returns false if member is not found
	SocketMapIterator found = _socketMap.find(name);
	if( found == _socketMap.end() )
		return false;

	// Otherwise, returns true.
	return true;
}

void SocketManager::destroyObject( Socket** socket )
{
	//if the socket exists
	if(*socket)
	{
		Socket* tempSocket = *socket;
		*socket = 0;
		// Shut down the socket.
		tempSocket->Shutdown( SD_BOTH );
		tempSocket->Close();
		// Deallocate memory.
		delete tempSocket;
	}
}