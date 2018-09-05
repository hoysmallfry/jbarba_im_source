#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include "AbstractThread.h"

#include "../Utilities/Users.h"

namespace Threads
{
	class ReceiveThread
		: public AbstractThread, public Users
	{
	private:
		//The user thread maintained by the accept thread.
		Users::Set* _userSet;

		// The name of the user.
		const std::string _username;

		//Internal thread that can access the string and user set.
		DWORD WINAPI realThread( void* parameter );

	public:
		// Assignment constructor.
		ReceiveThread( ThreadManager* threadManager, SocketManager* socketManager,
			const std::string username, Users::Set* userSet );
	};
}

#endif
