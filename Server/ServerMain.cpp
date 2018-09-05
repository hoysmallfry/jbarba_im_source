#include <exception>

#include "ServerProgram.h"

#include "../Utilities/Console.h"
#include "../Utilities/ErrorBox.h" // Display errors caught.
#include "../Sockets/SocketManager.h"

int main( void )
{
	try
	{
		Program _program;
		_program.Start();
	}
	catch( const std::exception& err )
	{
		// Displays the error in a pop-up
		Error::PopUp( err.what(), "Server Error!" );
	}

	return 0;
}
