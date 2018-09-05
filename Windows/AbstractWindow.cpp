// ---------------------------------------------------------------------------
// Project Name		:	Simple Framework
// File Name		:	AstractWindow.cpp
// Author			:	Joel Barba
// Creation Date	:	2009/10/11
// Purpose			:	Implements generic window.
// ---------------------------------------------------------------------------
#include "AbstractWindow.h"

#include <windowsx.h>// For the windows macros

namespace MyWindows
{
	static bool halting = false;

	//Assignment constructor.
	AbstractWindow::AbstractWindow( HWNDManager* hWndManager, HINSTANCE hInstance )
		: _hWndManager(hWndManager), _hInstance(hInstance) {}

	void AbstractWindow::Show( const std::string& windowName, int nShowCmd )
	{
		// Gets the hWnd based on name.
		HWND hWnd = getHWND( windowName );
		// Shows parent window.
		ShowWindow( hWnd, nShowCmd );
	}
	void AbstractWindow::Update( const std::string& windowName )
	{
		// Gets the hWnd based on name.
		HWND hWnd = getHWND( windowName );
		// Updates parent window.
		UpdateWindow( hWnd );
	}

	//Creates the parent window.
	void AbstractWindow::CreateParentWindow( const std::string& className, const std::string& name, DWORD style, int x, int y, int width,int height )
	{
		// The template name, the Actual instance name, position, scale, and process handle.
		HWND hWnd = CreateWindowEx( NULL, className.c_str(), name.c_str(), style, x, y, width, height, NULL, NULL, _hInstance, NULL );
		
		//If there was a problem, throw an error.
		if(!hWnd)
		{
			throw std::runtime_error( "Instantiating " + name + " failed!" );
		}

		// Otherwise, add to my window handle manager.
		_hWndManager->Add( name, hWnd );

		// Append a pointer to class behind the instance of the class. Will be used by external win proc.
		Registry& registry = Registry::GetRegistry();
		registry.Register( hWnd, this);
	}

	void AbstractWindow::CreateChildWindow( const std::string& type, const std::string& parentName, const std::string& postfix, DWORD style, int x, int y, int width, int height, const std::string& string )
	{
		// Gets the parent by name.
		HWND hWndParent = _hWndManager->Get( parentName );
		// Creates a new name based on parent and post fix (ex: joel_chatLogBox )
		std::string name = parentName + "_" + postfix;

		// Creates the window using the other necessary flags.
		HWND hWnd = CreateWindow( type.c_str(), string.c_str(), WS_CHILD | WS_VISIBLE | style, x, y, width, height, hWndParent, NULL, _hInstance, NULL );
		
		// If something goes wrong, throw error.
		if(!hWnd) throw "Instantiating " + name + " failed!";
		
		// Otherwise, add to my window handle manager.
		_hWndManager->Add( name , hWnd );
	}

	const std::string& AbstractWindow::getKey( const HWND& hWnd ) const
	{
		// Wrap around for the native hWndManager.
		return _hWndManager->GetKey(hWnd);
	}

	const HWND AbstractWindow::getHWND( const std::string& str ) const
	{
		// Wrap around for the native hWndManager.
		return _hWndManager->Get(str);
	}

	void AbstractWindow::removeHWND( const std::string& name )
	{
		_hWndManager->Remove( name );
	}

	HICON AbstractWindow::CustomIcon( int iconValue )
	{
		// Loads custom icon, returns custom icon.
		HICON hIcon = LoadIcon( _hInstance,MAKEINTRESOURCE(iconValue));
		return hIcon;
	}

	void AbstractWindow::showHide( const std::string& name, UINT flags )
	{
		HWND hWnd = getHWND( name );
		ShowWindow( hWnd, flags );
	}

	void AbstractWindow::setMove( const std::string& name, int x, int y, int width, int height, UINT flags )
	{
		HWND hWnd = getHWND( name );
		SetWindowPos( hWnd, NULL, x, y, width, height, flags );
	}

	RECT AbstractWindow::getMove( const std::string& name ) const
	{
		RECT rectangle;
		HWND hWnd = getHWND( name );
		GetWindowRect( hWnd, &rectangle );

		return rectangle;
	}

	void AbstractWindow::setText( const std::string& name, const std::string& text )
	{
		HWND hWnd = getHWND( name.c_str() );
		SetWindowText( hWnd, text.c_str() );
	}

	void AbstractWindow::clearText( const std::string& name )
	{
		setText( name, "" );
	}

	unsigned AbstractWindow::getTextLength( const std::string& name ) const
	{
		HWND hWnd = getHWND( name );
		int length = GetWindowTextLength( hWnd );
		return length;
	}

	std::string AbstractWindow::getText( const std::string& name ) const
	{
		int length = getTextLength(name) + 1;

		// Create a new character array
		char* buffer = new char[length];
		// Get the handle from the name
		HWND hWnd = getHWND( name );
		GetWindowText( hWnd, buffer, length );
		// Copy string
		std::string str = ( buffer );
		// Release allocated memory
		delete[] buffer;

		// Return the string.
		return str;
	}

	void AbstractWindow::enableWindow( const std::string& name, bool enabled )
	{
		// Enable the window given
		HWND hWnd = getHWND( name.c_str() );
		EnableWindow( hWnd, enabled );
	}

	void AbstractWindow::limitEditText( const std::string& name, const unsigned int size )
	{
		//Limit the maximum text that can be typed in the edit box.
		HWND hWnd = getHWND( name );
		Edit_LimitText( hWnd, size );
	}
	void AbstractWindow::scrollEditText( const std::string& name, const int vertical, const int horizontal )
	{
		HWND hWnd = getHWND( name );
		Edit_Scroll( hWnd, vertical, horizontal );
	}
	unsigned AbstractWindow::getEditLineCount( const std::string& name )
	{
		HWND hWnd = getHWND( name );
		return Edit_GetLineCount( hWnd );
	}

	//Yadda yadda, these are all straight-forward help functions.
	void AbstractWindow::addListBoxEntry( const std::string& name, const std::string& entry )
	{
		HWND hWnd = getHWND( name.c_str() );
		ListBox_AddString( hWnd, entry.c_str() );
	}

	int AbstractWindow::removeListBoxEntry( const std::string& name, int index )
	{
		HWND hWnd = getHWND( name.c_str() );
		int remaining = ListBox_DeleteString( hWnd, index );
		return remaining;
	}

	void AbstractWindow::emptyListBox( const std::string& name )
	{
		HWND hWnd = getHWND( name );
		BOOL ListBox_ResetContent( hWnd );
	}

	int AbstractWindow::findListBoxEntry( const std::string& name, const std::string& comparison )
	{
		HWND hWnd = getHWND( name.c_str() );
		int index = ListBox_FindStringExact( hWnd, -1, comparison.c_str() );
		return index;
	}

	std::string AbstractWindow::getListBoxEntry( const std::string& name, int index ) const
	{
		HWND hWnd = getHWND( name );
		char buffer[256];
		ListBox_GetText( hWnd, index, buffer );
		std::string str = buffer;

		return str;
	}

	int AbstractWindow::getListBoxCursorSelection( const std::string& name ) const
	{
		// Returns index of highlighted list box entry.
		HWND hWnd = getHWND( name );
		int index = ListBox_GetCurSel( hWnd );
		return index;
	}

	void AbstractWindow::halt_windows(void)
	{
		halting = true;
	}

	LRESULT CALLBACK AbstractWindow::DummyProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		Registry& registry = Registry::GetRegistry();
		AbstractWindow* iWindow = registry.GetWindow(hWnd);

		//If the pointer has yet to exist have the default proc handle it.
		if(!iWindow || halting)
			return DefWindowProc(hWnd, message, wParam, lParam);

		// Otherwise convert it to an AbstractWindow pointer and call the real procedure.
		LRESULT lResult = iWindow->Procedure( hWnd, message, wParam, lParam);

		return lResult;
	}
}
