#ifndef ACCEPTTHREAD_H
#define ACCEPTTHREAD_H

#include <vector>

#include "AbstractThread.h"

#include "../Utilities/Users.h"

namespace Threads
{
	class AcceptThread
		: public AbstractThread, public Users
	{
	private:
		typedef std::vector<AbstractThread*> ThreadVector;
		typedef ThreadVector::iterator ThreadVectorIterator;

		//Holds the names of the users, sets them in alphabeting order.
		Users::Set _userSet;
		//Holds the thread objects for the threads receiving data.
		ThreadVector _threadVector;

		//The internal thread that can use the data members
		DWORD WINAPI realThread( void* parameter );
		// Adds a list of names to list boxes in users when a new user joins.
		void addToSet( std::string& name, Socket prospect );

	public:
		// Sets up the pointers.
		AcceptThread( ThreadManager* threadManager, SocketManager* socketManager );
		// Destroys the user vector objects if they still exist.
		~AcceptThread( void );
	};
}

#endif
