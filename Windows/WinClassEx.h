#ifndef WINCLASSEX_H
#define WINCLASSEX_H

#undef _UNICODE
#undef UNICODE
#include <windows.h>

class WinClassEx
{
private:
	WNDCLASSEX winClassEx;

public:
	//Default constructor.
	WinClassEx( void );
	//Copy constructor.
	WinClassEx( const WinClassEx& copy );

	//Assignment constructor. assigns all the WNDCLASSEX members all at once.
	WinClassEx( LPCSTR _className, HINSTANCE _hInstance, WNDPROC _wndProc,
		HBRUSH _hbrBackground = CreateSolidBrush( RGB(255, 255, 255) ),
		HICON _hIcon = LoadIcon( NULL, IDI_APPLICATION),
		HICON _hIconSm = LoadIcon( NULL, IDI_APPLICATION),
		HCURSOR _hCursor = LoadCursor(NULL, IDC_ARROW),
		UINT _style = CS_HREDRAW | CS_VREDRAW,
		LPCSTR _lpszMenuName = NULL,
		UINT _size = sizeof(WNDCLASSEX),
		int _classExtra = 0, int _windowExtra = 0 );

	// Returns the single member as a constant.
	const WNDCLASSEX operator()( void ) const;
};

#endif
