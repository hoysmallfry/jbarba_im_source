#ifndef CLIENTPROGRAM_H
#define CLIENTPROGRAM_H

#include "../Sockets/Socket.h"

class Program
{
public:
	Program( void );
	~Program( void );
	void Start( HINSTANCE hInstance, int nShowCmd, MSG& msg );

private:
	void InitWinSock( void );
	Socket _socket;
};

#endif
