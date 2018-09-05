#include "WinClassEx.h"

//Default constructor.
WinClassEx::WinClassEx( void  ) {}

//Copy over the members.
WinClassEx::WinClassEx( const WinClassEx& copy )
: winClassEx( copy.winClassEx ) {}

//Set up the members.
WinClassEx::WinClassEx( LPCSTR _className, HINSTANCE _hInstance, WNDPROC _wndProc,
					   HBRUSH _hbrBackground, HICON _hIcon, HICON _hIconSm, 
					   HCURSOR _hCursor, UINT _style, LPCSTR _lpszMenuName, UINT _size,
					   int _classExtra, int _windowExtra )
{
	winClassEx.lpszClassName=_className;
	winClassEx.hInstance = _hInstance;
	winClassEx.lpfnWndProc = _wndProc;
	winClassEx.hbrBackground = _hbrBackground;
	winClassEx.hIcon = _hIcon;
	winClassEx.hIconSm = _hIconSm;
	winClassEx.hCursor = _hCursor;
	winClassEx.style = _style;
	winClassEx.lpszMenuName = _lpszMenuName;
	winClassEx.cbSize = _size ;
	winClassEx.cbClsExtra = _classExtra ;
	winClassEx.cbWndExtra = _windowExtra ;
}

// Return a constant reference to the native member.
const WNDCLASSEX WinClassEx::operator()( void ) const
{
	return winClassEx;
}
