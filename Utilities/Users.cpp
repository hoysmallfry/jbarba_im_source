#include "Users.h"

Users::Users( SocketManager* socketManager )
: _socketManager(socketManager) {}

void Users::SendDataPack( Socket& socket, const DataPack& dataPack )
{
	//send the array to the user as a datapack.
	socket.Send( &dataPack, sizeof(dataPack) );
}

void Users::SendDataPackToAll( const Vector& userVector, const DataPack& dataPack )
{
	//If the socketManager is assigned. Will do nothing if there is a null pointer assigned..
	if( _socketManager )
	{
		//send the user as a datapack to add user.
		VectorConstIterator iter = userVector.begin();
		//iterates until it finds last element ""
		while( *iter != "" )
		{
			//gets the user from the socket list,
			Socket* user = _socketManager->get( *iter );

			//Sends the datapack to the users with the new user as data.
			//When they recieve the message with the header they'll know what to do with it.
			SendDataPack( *user, dataPack );

			++iter;
		}
	}
}
