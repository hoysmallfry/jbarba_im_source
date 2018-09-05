#include "Socket.h"
#include <sstream>

Socket::Socket( void )
{
	// Sets up the default socket.
	_localSocket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0 );
	if( _localSocket == INVALID_SOCKET )
		// Gets the last error, passes it to my exception class which prints
		// the error out in the catch.
		throw WSAException( WSAGetLastError() );
}

// Copies contents over.
Socket::Socket( const Socket& copy )
: _localSocket(copy._localSocket){}
// Copies contents over.
Socket::Socket( const SOCKET& localSocket )
: _localSocket(localSocket) {}

//Defines a more customizable socket.
Socket::Socket( IN int af, int socketType, int protocol,
			   LPWSAPROTOCOL_INFOA protocolInfo, GROUP group, DWORD flags )
{
	_localSocket = WSASocket( af, socketType, protocol, protocolInfo, group, flags );
	if( _localSocket == INVALID_SOCKET )
		// Gets the last error, passes it to my exception class which prints
		// the error out in the catch.
		throw WSAException( WSAGetLastError() );
}

unsigned Socket::Number( void )
{
	// Returns the value of the socket as unsigned int.
	return static_cast<unsigned>(_localSocket);
}

void Socket::Bind( sockaddr_in socketAddress )
{
	// Binds the socket with address, checks for errors.
	if( bind( _localSocket, reinterpret_cast<sockaddr*>(&socketAddress),
		sizeof(socketAddress)) == SOCKET_ERROR )
			// Gets the last error, passes it to my exception class which prints
			// the error out in the catch.
			throw WSAException( WSAGetLastError() );
}
void Socket::Listen( IN int backlog )
{
	// Listens on socket, checks for errors.
	if( listen( _localSocket, backlog ) == SOCKET_ERROR )
		// Gets the last error, passes it to my exception class which prints
		// the error out in the catch.
		throw WSAException( WSAGetLastError() );
}
void Socket::Connect( sockaddr_in remoteSocket )
{
	//Attempts a connection with the remote address, checks for errors.
	if( connect( _localSocket, reinterpret_cast<sockaddr*>(&remoteSocket),
		sizeof(remoteSocket)) == SOCKET_ERROR )
			throw WSAException( WSAGetLastError() );
}
Socket Socket::Accept( void ) const
{
	//temp varaiables.
	SOCKET remoteSocket = 0;
	sockaddr_in remoteAddress;
	int remoteSize = sizeof(remoteAddress);
	SecureZeroMemory(&remoteAddress, remoteSize);

	//Attempt to accept.
	remoteSocket = accept( _localSocket, 
		reinterpret_cast<sockaddr*>(&remoteAddress), &remoteSize );

	// Check for errors.
	if( remoteSocket == INVALID_SOCKET )
		throw WSAException( WSAGetLastError() );

	//return the socket accepted.
	return remoteSocket;
}

void Socket::Send( const void* data, int size )
{
	int totalBytes = 0;
	while( totalBytes != size )
	{
		int bytesSent = send( _localSocket,
			reinterpret_cast<const char*>(data)+totalBytes, size-totalBytes, 0);
		
		// Send the data over of size with native member. Checks for any winsock errors
		if( bytesSent == SOCKET_ERROR )
			throw WSAException( WSAGetLastError() );

		totalBytes += bytesSent;
	}
}

int Socket::Receive( void* data, int size )
{
	// Attempts to receive data from remote socket.  
	int bytesRead = recv( _localSocket, reinterpret_cast<char*>(data), size, 0);
	
	// Checks for errors.
	if( bytesRead == SOCKET_ERROR )
	{
		// Checks for the case that socket is unblocking and needs to iterate through loop
		if( WSAGetLastError() == WSAEWOULDBLOCK )
		{
			Sleep( 1 );
			return 0;
		}
		else
			// Otherwise, throw error.
			throw WSAException( WSAGetLastError() );
	}

	//return number of bytes read.
	return bytesRead;
}
void Socket::Shutdown( int how )
{
	//Stops the socket from sending and receiving.
	if(_localSocket)
		shutdown(_localSocket, how);
}
void Socket::Close( void )
{
	//if non zero, close the socket
	if(_localSocket)
		closesocket(_localSocket);
}

void Socket::Block( void )
{
	// If its true then enabled is non sero and nonblocking mode is enabled
	// for the socket.
	u_long enabled = 0;
	ioctlsocket(_localSocket, FIONBIO, &enabled);
}

void Socket::Unblock( void )
{
	// If its true then enabled is non sero and nonblocking mode is enabled
	// for the socket.
	u_long enabled = 1;
	ioctlsocket(_localSocket, FIONBIO, &enabled);
}
