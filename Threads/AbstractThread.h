#ifndef ABSTRACTTHREAD_H
#define ABSTRACTTHREAD_H

#include "../Utilities/SqueakyCleanWindows.h"
#include "ThreadManager.h"

namespace Threads
{
	class AbstractThread
	{
	protected:
		//Needs to know its handle.
		HANDLE threadHandle;
		ThreadManager* _threadManager;

		//The internal thread that will be using the native members. Purely abstract.
		virtual DWORD WINAPI realThread( void* parameter ) = 0;

	public:
		//Constructor.
		AbstractThread( void );
		//Destructor.
		virtual ~AbstractThread( void ){}

		//Assignment constructor.
		AbstractThread( ThreadManager* threadManager );

		//External thread that accesses internal thread function.
		static DWORD WINAPI DummyThread( void* parameter );
	};
}

#endif
