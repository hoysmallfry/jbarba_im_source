#include "AcceptThread.h" 
#include "ReceiveThread.h" // will create receive threads.

#include "../Utilities/Serializer.h" // for making arrays from byte streams and such. 
#include "../Utilities/ErrorBox.h" // Display errors caught.
#include "../Utilities/Constants.h"
#include "../Utilities/Console.h"

namespace Threads
{
	AcceptThread::AcceptThread( ThreadManager* threadManager, SocketManager* socketManager )
			: AbstractThread( threadManager), Users( socketManager ) {}

	AcceptThread::~AcceptThread( void )
	{
		// Iterates through each thread object
		ThreadVectorIterator iter = _threadVector.begin();
		while( iter != _threadVector.end() )
		{
			// Deallocates the data for the object
			delete *iter;
			++iter;
		}

		//Clears the vector.
		_threadVector.clear();
	}

	DWORD WINAPI AcceptThread::realThread( void* parameter )
	{
		// Creates a client socket.
		Socket clientSocket;
		// Gets the listener socket from the Socket manager.
		Socket* listenerSocket = _socketManager->get("listenerSocket");

		try
		{
			// Will listener socket is still being used, 
			while(listenerSocket)
			{

				// Accept any attempts at connection
				clientSocket = listenerSocket->Accept();

				//Create a buffer
				char buffer[Constants::nameSize];
				//receive the first data send by the client: the username.
				int bytesRead = clientSocket.Receive( buffer, Constants::nameSize );
				//convert to string.
				std::string username(buffer);

				if(!bytesRead) continue; //If nothing was read, move on to the next iteration.

				//If a socket does not already exist in the socket manager with the username as a key, make one. 
				if( !_socketManager->isMember(username) )
				{
					//Set the console color to bright green.
					Console::SetConsoleColor( Colors::Green );
					//Notify the user of the server about what is happening.
					std::cout << "User " << username << "[" << clientSocket.Number() << "] has logged on." << std::endl;
					
					// Add the username to the set.
					addToSet( username, clientSocket );

					//instantiate a receive thread object, send it the pointers to the managers.
					ReceiveThread* receiveThread = new ReceiveThread( _threadManager, _socketManager, username, &_userSet );
					
					//push the object onto the vector to keep track of it.
					_threadVector.push_back( receiveThread );

					//Create the thread. The socket and the threads will be tracked by the user name.
					_threadManager->Create( username+"_ReceiveThread", NULL, NULL, receiveThread->DummyThread, receiveThread, NULL, NULL );
				}
				else
				{
					// If there is already a name, refuse the client by sending a data pack which forces him to disconnect.
					char message[] = "There is already a user logged on with that user name. Choose a different name.";
					DataPack disconectMessage( "Used Name!!", message, sizeof(message) );

					//Set the console color to red
					Console::SetConsoleColor( Colors::Red );
					//Notify the user of the server that a client was refused.
					std::cout << " User [" << clientSocket.Number() << "] refused due to name conflict: " << username << std::endl;

					//create a packet and send a disconnect error.
					SendDataPack( clientSocket, disconectMessage );
				}
			}
		}
		catch( const WSAException& err )
		{
			// Display any winsock errors.
			Error::PopUp( err.what(), "Accept Error!" );
		}

		return 0;
	}

	void AcceptThread::addToSet( std::string& name, Socket prospect )
	{
		// Convert the set to a vector.
		Users::Vector userVector = Serializer::CrunchitizeMeCaptain<std::string>( _userSet );

		// Add the user to the list.
		_socketManager->add( name, prospect );
		//insert to the set.
		_userSet.insert( name );

		int sizeStr = sizeof(std::string);
		size_t userNum = userVector.size();
		//Sends the datapack to the client.
		unsigned int rawSize = static_cast<int>(userNum)*sizeStr;
		//creates a data pack with the vector of usernames
		DataPack listPack( "AddUserToList", &userVector[0], rawSize );
		//sends the list of users to the connected socket, tells it to add the strings to his list box.
		Users::SendDataPack( prospect, listPack );

		//Tells the users that a new client has joined and they should add username to their list box. 
		Users::SendDataPackToAll( userVector, DataPack( "AddUserToList", &name, sizeof(std::string) ) );

	}
}
