#include "ServerProgram.h"

#include "../Threads/MyThreads.h"

#include "../Utilities/Console.h"

const int backLog = 10;

Program::Program( void )
{
	InitWinSock();
}

Program::~Program( void )
{
	//Closes and removes all sockets.
	_socketManager.removeAll();

	std::cout << "\nNo longer listening for connections . . . " << std::endl;

	//Cleans WinSock up.
	WSACleanup();

	Console::SetConsoleColor( Colors::Cyan );
	std::cout << "WSA environment ending." << std::endl;
}

// Starts the program.
void Program::Start()
{	
	// Prompt the user for the port number.
	int portnumber = AskForPort();

	// Create the local socket.
	sockaddr_in thisIPAddress = InitLocalSocket( portnumber );

	// Start the listener socket.
	InitListenerSocket( thisIPAddress );
	// Start the thread manager.
	InitThreadManager();
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

	//Set the console color to Cyan.
	std::cout << Console::SetColor(Colors::Cyan) << "WinSock environment starting." << std::endl;
}

// Prompts user for a port.
int Program::AskForPort( void )
{

	int portnumber;
	Console::SetConsoleColor( Colors::Gray );
	std::cout << "\nType in a port to listen to:" << std::endl;
	std::cin >> portnumber;
	return portnumber;
}

// Creates a local socket.
sockaddr_in Program::InitLocalSocket( int portNumber )
{
	hostent* localhost = gethostbyname("");
	char* localIP = inet_ntoa( *reinterpret_cast<in_addr*>(*localhost->h_addr_list));
	//Sets the destination to the server socket.
	sockaddr_in thisAddress;
	thisAddress.sin_family = AF_INET;
	thisAddress.sin_addr.s_addr = inet_addr(localIP);
	thisAddress.sin_port = htons(portNumber);

	std::cout << Console::SetColor(Colors::Yellow) 
		<< "\n\nLocal host name: " << localhost->h_name << "\n"
		<< "Local IP Address: " << localIP << "\n"
		<< "Port number: " << portNumber << std::endl;

	return thisAddress;
}

// Creates a socket for listening.
void Program::InitListenerSocket( sockaddr_in thisIPAddress )
{
	//Create the socket in the Manager.
	_socketManager.add( "listenerSocket" );
	//Get the socket from the manager.
	Socket* const listenerSocket = _socketManager.get( "listenerSocket" );

	//bind our listener socket to local endpoint
	listenerSocket->Bind( thisIPAddress );

	//Listen for connections.
	//Console::SetConsoleColor( Colors::White );
	std::cout << "\nListening for connections . . . " << std::endl;
	listenerSocket->Listen( backLog );
}

// Creates a thread manager and the initial thread. Blocks till all threads have died.
void Program::InitThreadManager( void )
{
	//Create a thread manager.
	Threads::ThreadManager threadManager;
	// Create a thread that waits for clients and accepts into the socket manager.
	Threads::AcceptThread acceptThread( &threadManager, &_socketManager );
	// Add the thread to the thread manager and start the thread.
	threadManager.Create( "AcceptThread", NULL, NULL, acceptThread.DummyThread, &acceptThread, NULL, NULL );
	//Wait for all threads to end.
	threadManager.WaitAll( TRUE, INFINITE );
}
