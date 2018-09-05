#include "ThreadManager.h"

namespace Threads
{
	//Closes all the handles
	ThreadManager::~ThreadManager( void )
	{
		if(!_threadHandles.empty() )
			RemoveAll();
	}

	void ThreadManager::Create( const std::string& name, LPSECURITY_ATTRIBUTES _securityAttr, SIZE_T _stackSize,
				threadFunct _thread, void* _parameter, DWORD _creationFlag, LPDWORD _threadID )
	{
		//If there is no preexisting thread under the name, create one.
		if( _threadHandles.find( name ) == _threadHandles.end() )
		{
			_threadHandles[name] = CreateThread( _securityAttr, _stackSize, _thread,
												_parameter, _creationFlag, _threadID );
		}
		//Otherwise, throw an error.
		else
		{
				throw "There is already an existing thread under the name " + name + ".";
		}
	}

	void ThreadManager::WaitSingle( const std::string& name, DWORD time )
	{
		//Wait for the single handle specified by the name
		WaitForSingleObject( *findHandle( name ), time );
	}

	void ThreadManager::WaitAll( BOOL waitForAll, DWORD time )
	{
		//Create an array of thread handles.
		HANDLE* threadArray = createThreadArray();
		//Use the array and pass it to wait for all items.
		WaitForMultipleObjects( (DWORD)_threadHandles.size(), threadArray, waitForAll, time );
		//Delete thread array after use.
		destroyThreadArray( threadArray );
	}

	void ThreadManager::CloseSingle( const std::string& name )
	{
		//Close a single handle.
		CloseHandle( *findHandle( name ) );
	}

	void ThreadManager::CloseAll( void )
	{
		//Close all the handles in the map.
		HandleMapIterator iter = _threadHandles.begin();
		while( iter != _threadHandles.end() )
		{
			CloseHandle( iter->second );
			++iter;
		}
	}

	void ThreadManager::Remove( const std::string& name )
	{
		//Close the handle given first.
		CloseHandle( *findHandle( name ) );
		//erase from the map.
		_threadHandles.erase( name );
	}

	void ThreadManager::RemoveAll( void )
	{
		//Close all handles first.
		CloseAll();
		//Clear the map.
		_threadHandles.clear();
	}

	HANDLE* ThreadManager::createThreadArray( void )
	{
		//Allocate memory for a array of the map member size
		HANDLE* threadArray = new HANDLE[_threadHandles.size() ];
		// Iterate through each member.
		HandleMapIterator iter = _threadHandles.begin();
		for( int index = 0; iter != _threadHandles.end(); index++, iter++ )
		{
			//copy the values over to the array
			*(threadArray+index) = iter->second;
		}
		//return the filled array.
		return threadArray;
	}

	void ThreadManager::destroyThreadArray( HANDLE* threadArray )
	{
		//delete the array after use.
		delete[] threadArray; 
	}

	HANDLE* ThreadManager::findHandle( const std::string& name )
	{
		HandleMapIterator found = _threadHandles.find( name );
		// If there is no thread under the name throw an error.
		if( found == _threadHandles.end() )
			throw "There is no thread under the name " + name + ".";

		// Otherwise, return the handle pointer.
		return &found->second;
	}
}
