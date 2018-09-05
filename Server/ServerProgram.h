#ifndef SERVERPROGRAM_H
#define SERVERPROGRAM_H

#include "../Sockets/SocketManager.h"

class Program
{
public:
	Program( void );
	~Program( void );
	void Start( void );

private:
	void InitWinSock( void );
	int AskForPort( void );
	sockaddr_in InitLocalSocket( int portNumber );
	void InitListenerSocket( sockaddr_in thisIPAddress );
	void InitThreadManager( void );

	SocketManager _socketManager;
};

#endif
