#include "AbstractThread.h"

namespace Threads
{
	// Pointers point to nothing if not assigned
	AbstractThread::AbstractThread( void )
		: _threadManager(0) {}

	// Assign pointers.
	AbstractThread::AbstractThread( ThreadManager* threadManager )
	: _threadManager(threadManager) {}

	// Default dummy thread asks for the class as a parameter to access all the members.
	DWORD WINAPI AbstractThread::DummyThread( void* parameter )
	{
		// Cast to abstract thread pointer.
		AbstractThread* iThread = reinterpret_cast<AbstractThread*>(parameter);

		//First validifies thread, then executes internal thread task.
		if(iThread)
			iThread->realThread( 0 );

		return 0;
	}
}
