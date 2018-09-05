#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <map>
#include <string>

namespace Threads
{
	class ThreadManager
	{
	private:
		// Thread function definition.
		typedef DWORD (WINAPI *threadFunct)( void* parameter );
		// Map of thread handles
		typedef std::map< std::string, HANDLE > HandleMap;
		// Iterator for thread handle iterator.
		typedef HandleMap::iterator HandleMapIterator;

		HandleMap _threadHandles;

		// Helper functions:
		// Creates an array of thread handles 
		HANDLE* createThreadArray( void );
		// Deletes the array of thread handles. 
		void destroyThreadArray( HANDLE* threadArray );
		// Finds a handle in the map with the corresponding key name.
		HANDLE* findHandle( const std::string& name );
		// Closes a single handle paired with the key name.
		void CloseSingle( const std::string& name );
		// Closes all handles in the map.
		void CloseAll( void );

	public:
		// Closes all the handles
		~ThreadManager( void );

		// Creates a thread, adds handle to map.
		void Create( const std::string& name, LPSECURITY_ATTRIBUTES _securityAttr, SIZE_T _stackSize,
			threadFunct _thread, void* _parameter, DWORD _creationFlag, LPDWORD _threadID );

		// Will wait for a thread whose handle is paired with a name, for time seconds
		void WaitSingle( const std::string& name, DWORD time );
		// Will wait for all elements in the map for time seconds.
		void WaitAll( BOOL waitForAll, DWORD time );

		// Will remove handle with matching key name.
		void Remove( const std::string& name );
		// Removes all handles in the map.
		void RemoveAll( void );
	};
}

#endif
