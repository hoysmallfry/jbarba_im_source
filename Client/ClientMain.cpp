#include <stdlib.h> //the usual stuff
#include <string.h>

#include "ClientProgram.h"
#include "../Utilities/ErrorBox.h" //Makes a messageBox for displaying errors in catches.

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	//Used for message handling
	MSG msg;
	msg.wParam = 0;//initialize at zero, in case there's an error before the while loop.

	try
	{
		Program program;
		program.Start( hInstance, nShowCmd, msg );
	}
	catch( const WSAException& err )
	{
		//handle any winsock errors
		Error::PopUp( err.what(), "Client Error!" );	
	}

	return static_cast<int>(msg.wParam);
}
