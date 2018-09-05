#include "ReceiveThread.h"

#include "../Utilities/Serializer.h"
#include "../Utilities/Constants.h"
#include "../Utilities/MessagePack.h"
#include "../Utilities/ErrorBox.h" // Display errors caught.
#include "../Utilities/Console.h"

namespace Threads
{
	ReceiveThread::ReceiveThread( ThreadManager* threadManager, SocketManager* socketManager,
		const std::string username, Users::Set* userSet )
		: AbstractThread( threadManager), Users( socketManager ),
		_username(username), _userSet(userSet) {}

	DWORD WINAPI ReceiveThread::realThread( void* parameter )
	{
		// Create a data buffer.
		char buffer[Constants::dataPackSize];
		
		// Get the user socket from the user name.
		Socket* userSocket = _socketManager->get( _username );

		// Start unblocking the socket.
		userSocket->Unblock();

		try
		{
			while( userSocket )
			{
				int bytesRead = userSocket->Receive( buffer, Constants::dataPackSize );

				//if there was no message (or there was a would block), continue
				if(!bytesRead) continue;

				// Convert the buffer into a data pack.
				DataPack packReceived( buffer );

				std::string header = packReceived.header;
				// Inform each user they should remove the disconnecting user from their list.
				if( header == "UserDisconnect" )
				{
					// Set the console text a dark green
					Console::SetConsoleColor( Colors::DarkGreen );

					// Notify the user server side to what's going on.
					std::cout << "User " << _username << "[" << userSocket->Number() << "] has disconnected." << std::endl;
					_userSet->erase( _username );

					// Iterate through the other users.
					Users::SetIterator iter = _userSet->begin();
					while( iter != _userSet->end() )
					{
						// Get the socket that matches the name.
						Socket* user = _socketManager->get( *iter );

						// Create a data pack with the message to remove from the list.
						DataPack removePack( "RemoveFromList", _username.c_str(), static_cast<unsigned int>(_username.length())+1 );
						
						// Send the data pack to the socket.
						Users::SendDataPack( *user, removePack );

						// Move on to the next user.
						++iter;
					}

					//breaks the while loop, ends the thread since the use is disconnecting.
					break;
				}
				else if( header == "AddToChatLog" )
				{
					//Create a message pack with the data received.
					MessagePack messagePack( packReceived.data );

					//Gets the socket based on who the message is being sent to.
					Socket* recipient = _socketManager->get( messagePack.To );
					
					// Set the console color to bright green
					Console::SetConsoleColor( Colors::Green );
					// Notify the user server side to what just happened.
					std::cout << "Routing message from " << messagePack.From << "[" << userSocket->Number() << 
						"] to " << messagePack.To << "[" << recipient->Number() << "]." << std::endl;

					//send the data pack to the recipient.
					recipient->Send( &packReceived, sizeof(packReceived) );
				}
			}
		}
		catch( const WSAException& error )
		{
			//Catch a winsock error and display it.
			Error::PopUp( error.what(), "Client Receive Error!" );
		}

		//Start blocking the socket again.
		userSocket->Block();
		
		//remove the user from the manager and the list.
		_socketManager->remove( _username );

		//destroy the handle to the thread, remove it from the list.
		_threadManager->Remove( _username+"_ReceiveThread" );

		return 0;
	}
}
