#ifndef USERS_H
#define USERS_H

#include <string>
#include <vector> //for the user vector.
#include <set>

#include "DataPack.h"

#include "../Sockets/SocketManager.h"
#include "../Sockets/Socket.h"

class Users
{
public:
	Users( SocketManager* socketManager );

	// Used by sever for user list.
	typedef std::set<std::string> Set;
	typedef Set::iterator SetIterator;
	typedef Set::const_iterator SetConstIterator;

	// Used by server when server takes list and makes dynamic array for stream.
	typedef std::vector<std::string> Vector;
	typedef Vector::iterator VectorIterator;
	typedef Vector::const_iterator VectorConstIterator;

protected:
	//Accepts a socket and data packet. Sends the data pack through the socket.
	void SendDataPack( Socket& socket, const DataPack& dataPack );

	//Takes a vector of names, takes the corresponding elements in socketManager, sends the data pack through each.
	void SendDataPackToAll( const Vector& vector, const DataPack& dataPack );

	SocketManager* _socketManager;
};

#endif
