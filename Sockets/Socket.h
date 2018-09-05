#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <string>

#include "../Utilities/SqueakyCleanWindows.h"

#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "../Utilities/WSAException.h" //An error class for winsock error codes

class Socket
{
private:
	// member socket.
	SOCKET _localSocket;

public:	
	// Default constructor. Creates a TCP compliant socket.
	Socket( void );
	
	// Copy constructor
	Socket( const Socket& copy );
	
	// Assignment constructor. takes a SOCKET and copies it.
	Socket( const SOCKET& localSocket );

	// Assignment constructor. Manually creates a socket.
	Socket(  IN int af, int socketType, int protocol,
		LPWSAPROTOCOL_INFOA protocolInfo, GROUP group, DWORD flags );

	// Gives use the number of the Socket. Used primarily by the server.
	unsigned Number( void );

	// Bonds a socket to an address.
	void Bind( sockaddr_in socketAddress );

	// Listens for a connection on _localSocket.
	void Listen( IN int backlog );

	// Connects to a remote socket address given.
	void Connect( sockaddr_in remoteSocket );
	// Accepts a request for connection.
	Socket Accept( void ) const;
	
	// Sends given data of given size to remote socket.
	void Send( const void* data, int size );

	// Receives given data of given size from remote socket.
	int Receive( void* data, int size );
	
	// Shuts down this socket. Asks for a method (SD_BOTH, SD_READ)
	void Shutdown( int how );
	// Closes this socket.
	void Close( void );

	//Blocks this socket.
	void Block( void );
	//Unblocks this socket.
	void Unblock( void );
};

#endif
