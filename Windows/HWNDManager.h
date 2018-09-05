#ifndef HWNDMANAGER_H
#define HWNDMANAGER_H

#include "../Utilities/SqueakyCleanWindows.h"
#include <map>
#include <string>

class HWNDManager
{
private:
	// Map of strings to window handles and an iterator.
	typedef std::map<std::string, HWND> HWNDMap;
	typedef HWNDMap::iterator HWNDMapIterator;
	HWNDMap _hWndMap;

	// Helper function. finds the window based on name.
	HWND findHWND( const std::string& name );

public:
	// Destructor. Clears the map.
	~HWNDManager( void );

	// Adds a new entry to the manager under a key name.
	void Add( const std::string& name, HWND windowHandle );

	// Retrieves a window handle with the key name.
	HWND Get( const std::string& name );
	// Retrieves the first key name that matches the value passed.
	const std::string& GetKey( HWND value );

	//Removes a single handle value paired with key name passed.
	void Remove( const std::string& name );
	// Removes all entries in the map.
	void RemoveAll( void );

	// Determines if there is a member paired with the key value passed.
	bool isMember( const std::string& name );
};

#endif
