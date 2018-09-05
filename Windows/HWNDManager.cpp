#include "HWNDManager.h"

HWNDManager::~HWNDManager( void )
{
	//Remove all if not empty
	if( !_hWndMap.empty() )
		RemoveAll();
}

void HWNDManager::Add( const std::string& name, HWND windowHandle )
{
	// Checks first if there is already a member by that name.
	if( isMember( name ) )
		throw name + " is already a member of the HWNDManager.";

	//Otherwise, adds element.
	_hWndMap[name] = windowHandle;
}

HWND HWNDManager::Get( const std::string& name )
{
	return findHWND(name);
}

const std::string& HWNDManager::GetKey( HWND value )
{
	// Iterates through map until finds first instance that matches value.
	HWNDMapIterator iter = _hWndMap.begin();
	while( iter != _hWndMap.end() )
	{
		if( iter->second == value )
			return iter->first;

		++iter;
	}
	//If goes through without finding a member, throw an error.
	throw "HWND Value not a member of HWNDManager.";
}

void HWNDManager::Remove( const std::string& name )
{
	// If is not member, throw an error.
	if( !isMember( name ) )
		throw name + " is not a member of the HWNDManager.";

	// Otherwise, erase member.
	_hWndMap.erase(name); 
}

void HWNDManager::RemoveAll( void )
{
	// Clear
	_hWndMap.clear();
}

//Will return a hWnd value if there is a member under the name
HWND HWNDManager::findHWND( const std::string& name )
{
	// Find member and if not member, throw error.
	HWNDMapIterator found = _hWndMap.find( name );
	if( found == _hWndMap.end() )
		throw name + " is not a member of the HWNDManager.";

	// Otherwise, return member.
	return found->second;
}

// Checks if there is a member with that name.
bool HWNDManager::isMember( const std::string& name )
{
	//Find member, return true if in map
	HWNDMapIterator found = _hWndMap.find( name );
	if( found == _hWndMap.end() )
		return false;

	//Otherwise, return false.
	return true;
}
