#include "ClientProgram.h"

#include "../Utilities/SqueakyCleanWindows.h" //lean, mean, unicode-less grilling machine.

#include "../Windows/MyWindows.h" //Contains the different window definitions
#include "../Windows/HWNDManager.h" //Keeps track of all the instances of windows by IDs

#include "../Threads/MyThreads.h"
#include "../Threads/ThreadManager.h" //Creates threads and keeps track of all the handles.

#include "../Sockets/Socket.h" //Carries all the functionality of a socket in a wraparound class.

const int backLog = 10;

Program::Program( void )
: _socket(0)
{
	InitWinSock();
}

Program::~Program( void )
{
	// Close our socket before ending.
	_socket.Shutdown( SD_BOTH );
	_socket.Close();

	//close the winsock environment
	WSACleanup();
}

// Starts the program.
void Program::Start( HINSTANCE hInstance, int nShowCmd, MSG& msg )
{	
	//Create a thread manager, a window handle manager and a empty socket.
	Threads::ThreadManager threadManager;
	HWNDManager hWndManager;



	//Create a window object that has pointers to our main data structs.
	MyWindows::UserListWindow userListWindow( &threadManager, &hWndManager, hInstance, &_socket );
	//Create a window using our window object, show it and update it.
	userListWindow.Instantiate( "UserListWindow" , 500, 300 );
	userListWindow.Show( "UserListWindow", nShowCmd);
	userListWindow.Update( "UserListWindow" );

	// Listen for messages that the operating system sends.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

// Create a WinSock environment.
void Program::InitWinSock( void )
{
	// Declare WinSock Data variable.
	WSADATA wsaData;
	// Zero out wsaData.
	SecureZeroMemory(&wsaData, sizeof(wsaData));
	
	//Initialize WinSock.
	if( WSAStartup(MAKEWORD(2,2), &wsaData) )
		throw WSAException( WSAGetLastError() );
}
