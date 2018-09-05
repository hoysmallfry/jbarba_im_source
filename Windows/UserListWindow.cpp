#include "UserListWindow.h"
#include "../Utilities/DataPack.h"
#include "../Utilities/Serializer.h"
#include "../Utilities/Constants.h"

const unsigned disconnectedWidth = 250;
const unsigned disconnectedHeight = 250;
const unsigned connectedWidth = 250;
const unsigned connectedHeight = 570;

const unsigned disconnectedPosX = (Constants::centerX)-(disconnectedWidth/2);
const unsigned disconnectedPosY = (Constants::centerY)-(disconnectedHeight/2);
const unsigned connectedPosX = (Constants::screenWidth)-(connectedWidth);
const unsigned connectedPosY = (Constants::centerY)-(connectedHeight/2);

namespace MyWindows
{
	UserListWindow::UserListWindow( Threads::ThreadManager* threadManager, HWNDManager* hWndManager, HINSTANCE hInstance, Socket* socket )
		: AbstractThread( threadManager ), AbstractWindow( hWndManager, hInstance ), _socket(socket)
	{
		//Defines the template of our user list window.
		_winClassEx = WinClassEx( "UserListWindow", _hInstance, DummyProcedure, CreateSolidBrush(RGB(0,180,0)),
			CustomIcon(ME_ICON), CustomIcon(ME_ICON) );

		// Registers the class under its name.
		RegisterClassEx( &_winClassEx() );

		// Is not connected at start.
		_connected = false;
	}

	//will destroy the chat window vector.
	UserListWindow::~UserListWindow( void )
	{
		//if the memory isn't freed, free it.
		DeleteAllChatWindows();
	}


	void UserListWindow::Instantiate( const std::string& windowName, const unsigned& posX, const unsigned& posY )
	{

		// Creates the parent window.
		CreateParentWindow( _winClassEx().lpszClassName, windowName, WS_SYSMENU,
			disconnectedPosX, disconnectedPosY, disconnectedWidth, disconnectedHeight );

		// Does an alpha blending animation upon  creation.
		AnimateWindow( getHWND(windowName), 250, AW_BLEND );

		//Define the child windows.
		CreateChildWindow( "STATIC", windowName, "UsernameTitle", NULL, 10, 10, 220, 20, "User name:" );
		CreateChildWindow( "EDIT", windowName, "Username", WS_BORDER, 10, 30, 220, 20 );
		CreateChildWindow( "STATIC", windowName, "IPAddressTitle", NULL, 10, 60, 220, 20, "IP Address:" );
		CreateChildWindow( "EDIT", windowName, "IPAddress", WS_BORDER, 10, 80, 220, 20 );
		CreateChildWindow( "STATIC", windowName, "PortTitle", NULL, 10, 110, 220, 20, "Port Number:" );
		CreateChildWindow( "EDIT", windowName, "Port", WS_BORDER, 10, 130, 220, 20 );
		CreateChildWindow( "BUTTON", windowName, "Connect", WS_BORDER | BS_DEFPUSHBUTTON, 10, 165, 220, 30 );
		CreateChildWindow( "STATIC", windowName, "UserListTitle", NULL, 10, 10, 220, 20, "Users:" );
		CreateChildWindow( "LISTBOX", windowName, "UserList", LBS_STANDARD | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL , 10, 30, 220, 300 );
		
		//Starts the window in disconnected mode.
		disconnectedWindowSettings( windowName );
		
		//limits the text in the edit box to 20.
		limitEditText( windowName+"_Username", Constants::nameSize );
		//disables the button until the edit boxes are filled.
		enableWindow( windowName+"_Connect", false );
	}

	void UserListWindow::Show( const std::string& windowName, int nShowCmd )
	{
		// Gets the hWnd based on name.
		HWND hWnd = getHWND( windowName );
		// Shows parent window.
		ShowWindow( hWnd, nShowCmd );
	}
	void UserListWindow::Update( const std::string& windowName )
	{
		// Gets the hWnd based on name.
		HWND hWnd = getHWND( windowName );
		// Updates parent window.
		UpdateWindow( hWnd );
	}

	LRESULT CALLBACK UserListWindow::Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		//The structure that used when painting
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		// Child window messages.
		case WM_COMMAND:
			handleChildMessages( hWnd, wParam, lParam);
			break;

		//If the procedure is told to open a chat window.
		case JBIM_CHATOPEN:
			MakeChatWindow( hWnd, lParam );
			break;

		//If the procedure is told to close a chat window.
		case JBIM_CHATCLOSE:
			//Removes the hWnd from the map.
			DestroyChatWindow( (HWND)lParam );
			break;

		case JBIM_DISCONNECT:
			disconnect( hWnd );
			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			endProgram( hWnd );
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		return 0;
	}

	void UserListWindow::handleChildMessages( const HWND& hWnd, const WPARAM& wParam, const LPARAM& lParam )
	{
		switch( HIWORD(wParam) )
		{
			// If the edit boxes are modified
		case EN_CHANGE:
			handleEditBoxMessages( hWnd, lParam );
			break;
		// If the list box is double clicked. (List box only visible if connected.)
		case LBN_DBLCLK:
			handleListBoxMessages( hWnd, lParam );
			break;
		//If the Connect button is clicked:
		case BN_CLICKED:
			handleButtonMessages( hWnd, lParam );
			break;
		}
	}

	void UserListWindow::handleEditBoxMessages( const HWND& hWnd, const LPARAM& lParam )
	{
		//gets the name of the parent window for use to access other window members 
		//(children have parent name prefix)
		std::string parentKey = getKey(hWnd);

		//boolean that determines if the connect button is enabled.
		bool enabled;
		//if any of the edit boxes are empty, disable connect.
		if( editBoxesEmpty( parentKey ) ) enabled = false;
		// Otherwise, enable it.
		else enabled = true;

		// Useability based on enabled.
		enableWindow( parentKey+"_Connect", enabled );
	}

	bool UserListWindow::editBoxEmpty( const std::string& windowName )
	{
		// If the text length is not zero for windoe specified, it is not empty so return false.
		if( getTextLength( windowName ) )
			return false;

		// Otherwise, return true.
		return true;
	}

	bool UserListWindow::editBoxesEmpty( const std::string& parent )
	{
		// If the username box is empty, return true.
		if( editBoxEmpty( parent+"_Username" ) )
			return true;
		if( editBoxEmpty( parent+"_IPAddress" ) )
			return true;
		if( editBoxEmpty( parent+"_Port" ) )
			return true;

		//If none are empty, return false.
		return false;
	}

	void UserListWindow::handleListBoxMessages( const HWND& hWnd, const LPARAM& lParam )
	{
		//gets the name of the parent window for use to access other window members 
		//(children have parent name prefix)
		std::string parentKey = getKey(hWnd);

		// Get the index of the highlighted entry.
		int index = getListBoxCursorSelection( parentKey+"_UserList" );
		// Use the index to get the name of the user.
		std::string remoteUser = getListBoxEntry( parentKey+"_UserList", index );
		// Gets the username of this user from the username edit box. ( Username edit box disabled if connected.)
		std::string localUser = getText( parentKey+"_Username" );

		// Create a new window if there was no empty click. Will not do anything if there already is a window open.
		if(remoteUser != "" )
			MakeChatWindow( parentKey, localUser, remoteUser );

	}

	void UserListWindow::handleButtonMessages( const HWND& hWnd, const LPARAM& lParam )
	{
		//gets the name of the parent window for use to access other window members (children have parentname prefix)
		std::string parentKey = getKey(hWnd);

		//If not connected, connect and change the layout of the window when disconnect connect button is pressed.
		if( !_connected )
		{
			connect( hWnd, parentKey );
		}
		//If connected when disconnect/connect button is pressed.
		else
		{
			SendMessage( hWnd, JBIM_DISCONNECT, NULL, NULL );
		}

	}

	void UserListWindow::connect( const HWND& hWnd, const std::string& parentKey )
	{
		//Assigns a new socket value to the pointer 
		*_socket = Socket();
		//Get the strings from the three text boxes
		std::string username = getText( parentKey+"_Username" );
		std::string address = getText( parentKey+"_IPAddress");
		std::string port = getText( parentKey+"_Port");

		//Here we create a socket address.
		sockaddr_in remoteAddr;
		//Here we tell the remote it will use TCP.
		remoteAddr.sin_family = AF_INET;
		//Here we set the remore address manually.
		remoteAddr.sin_addr.s_addr = inet_addr( address.c_str() );
		// open up at address given. We use htons to format this in network format regardless of the machine's endian-ness
		remoteAddr.sin_port = htons( atoi(port.c_str()) );

		try
		{
			// Sets up the the connecting mode until the window is connected.
			connectingWindowSettings( parentKey );
			// Connects the socket to the remote address.
			connectSocket( remoteAddr );
			// Sets the connected settings.
			connectedWindowSettings( parentKey );

			//Now connected.
			_connected = true;

			//Sends this users username so server can save the socket under the username.
			std::string username = getText( parentKey+"_Username" );
			_socket->Send( username.c_str(), Constants::nameSize );

			//Id sending successful, create a thread for receiving data from the server.
			_threadManager->Create( "ReceiveThread", NULL, NULL, DummyThread, hWnd, NULL, NULL );
		}
		catch( const WSAException& error )
		{
			// Display an error if there was any problems in the initial connect or send. 
			Error::PopUp( error.what(), "Client Connection Error!" );
			// After displaying error, go back to disconnect mode.
			_connected = false;
			disconnectedWindowSettings( parentKey );
		}
	}

	void UserListWindow::disconnect( const HWND& hWnd )
	{
		// gets the name of the parent window for use to access other window members
		// (children have parent name prefix)
		std::string parentKey = getKey(hWnd);

		try
		{
			// Set as disconnected. Will end the recieve thread.
			_connected = false;

			// Wait for receive thread to be done with its last task.
			_threadManager->WaitSingle( "ReceiveThread", INFINITE );

			//The handle to the thread will be closed, thread will terminate.
			_threadManager->Remove( "ReceiveThread" );

			// Set the disconnecting mode until finally disconnected.
			disconnectingWindowSettings( parentKey );
			// Disconnect the socket.
			disconnectSocket( parentKey );
			// Set the window back to disconnected mode.
			disconnectedWindowSettings( parentKey );
		}
		catch( const WSAException& error )
		{
			// Display an error if there was any problems in the disconnecting. 
			Error::PopUp( error.what(), "Client Disconnection Error!" );
			// After displaying error, go back to disconnect mode.
			_connected = false;
			disconnectedWindowSettings( parentKey );
		}
	}

	void UserListWindow::endProgram( const HWND& hWnd )
	{
		//gets the name of the parent window for use to access other window members (children have parentname prefix)
		std::string parentKey = getKey(hWnd);
		//disconnect if connected before destroying window by simulating a button click on the disconnect button.
		if(_connected)
			SendMessage( hWnd, JBIM_DISCONNECT, NULL, NULL );

		//Remove all the child windows from the Hwnd manager.
		removeHWND( parentKey + "_UserList" );
		removeHWND( parentKey + "_UserListTitle" );
		removeHWND( parentKey + "_Connect" );
		removeHWND( parentKey + "_Port" );
		removeHWND( parentKey + "_PortTitle" );
		removeHWND( parentKey + "_IPAddress" );
		removeHWND( parentKey + "_IPAddressTitle" );
		removeHWND( parentKey + "_Username" );
		removeHWND( parentKey + "_UsernameTitle" );

		// The window is terminated. Send a message to yourself to end the application.
		PostQuitMessage(0);
	}


	void UserListWindow::MakeChatWindow( const HWND& hWnd, const LPARAM& lParam )
	{
		//gets the name of the parent window for use to access other window members (children have parentname prefix)
		std::string parentKey = getKey(hWnd);
		//Takes the param pointer and recast it to the message pack.
		MessagePack* messagePack = reinterpret_cast<MessagePack*>(lParam);
		// Makes a chat window. does not make one if already made.
		MakeChatWindow( parentKey, messagePack->To, messagePack->From );
	}
	void UserListWindow::MakeChatWindow( const std::string& parentKey, const std::string& localUser, const std::string& remoteUser  )
	{
		// Checks to see if the window object isn't already made.
		if( _chatWindows.find(remoteUser) == _chatWindows.end() )
		{
			// If not a user, adds a user.
			ChatWindow* chatWindow = new ChatWindow( _hWndManager, _winClassEx().hInstance,  _socket, parentKey, localUser, remoteUser );
			// Adds it to the map.
			_chatWindows[remoteUser] = chatWindow;
		}

		// Check if there is already a user under the name.
		if( !_hWndManager->isMember(remoteUser) )
		{
			// Instantiates and shows window.
			_chatWindows[remoteUser]->Instantiate( remoteUser, 100, 100 );
			_chatWindows[remoteUser]->Show( remoteUser, TRUE );
			_chatWindows[remoteUser]->Update( remoteUser );
		}

	}

	void UserListWindow::DestroyChatWindow( HWND hWnd )
	{
		// Removes from the hwnd manager.
		std::string chatWindowName = getKey( hWnd );
		removeHWND( chatWindowName );
	}

	void UserListWindow::DestroyAllChatWindows( void )
	{
		// Destroys all the chat windows.
		ChatWindowIterator iter = _chatWindows.begin();
		while( iter != _chatWindows.end() )
		{
			//If the name of the chat window matches any name in the hWndManager, there is an instance, so remove it.
			if( _hWndManager->isMember(iter->first) )
				DestroyWindow( getHWND(iter->first) );

			++iter;
		}
	}

	void UserListWindow::DeleteAllChatWindows( void )
	{
		// Destroys all the chat windows.
		ChatWindowIterator iter = _chatWindows.begin();
		while( iter != _chatWindows.end() )
		{
			// Deletes all the objects from the map. 
			delete iter->second;
			++iter;
		}
		//Clear the map.
		_chatWindows.clear();
	}

	void UserListWindow::connectSocket( sockaddr_in remote )
	{
		// Connect the client socket.
		_socket->Connect( remote );
	}

	void UserListWindow::disconnectSocket( const std::string& parentKey  )
	{
		// Sends the server a message that the socket is about to disconnect.
		DataPack disconnectMessage( "UserDisconnect", 0, 0 );
		// Send the array to the user as a datapack with a "add user list" header.
		_socket->Send( &disconnectMessage, sizeof(disconnectMessage) );

		// Shutdown and close the socket.
		_socket->Shutdown( SD_BOTH );
		_socket->Close();
	}

	void UserListWindow::connectingWindowSettings( const std::string& parentKey )
	{
		// Set up the connected settings, disable edit boxes and the button
		enableWindow( parentKey+"_Username", false );
		enableWindow( parentKey+"_IPAddress", false );
		enableWindow( parentKey+"_Port", false );
		enableWindow( parentKey+"_Connect", false );
		// Set the text on the button
		setText( parentKey+"_Connect", "Connecting...");
	}

	void UserListWindow::connectedWindowSettings( const std::string& parentKey )
	{
		//Move the parent window to the side.
		setMove( parentKey, connectedPosX, connectedPosY, connectedWidth, connectedHeight );
		//Move the child windows around.
		setMove( parentKey+"_UsernameTitle", 10, 350, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_Username", 10, 370, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_IPAddressTitle", 10, 400, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_IPAddress", 10, 420, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_PortTitle", 10, 450, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_Port", 10, 470, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_Connect", 10, 500, 0, 0, SWP_NOSIZE );

		//Show the list box and title.
		showHide( parentKey+"_UserListTitle", SW_SHOWNORMAL );
		showHide( parentKey+"_UserList", SW_SHOWNORMAL );

		// Re-enable the button.
		enableWindow( parentKey+"_Connect", true );
		// Reset the text on the button.
		setText( parentKey+"_Connect", "Disconnect" );
	}

	void UserListWindow::disconnectingWindowSettings( const std::string& parentKey )
	{
		// Get rid of any instances that exist.
		DestroyAllChatWindows();

		// Clear all the entries in the list box. we don't want doubles when we reconnect.
		emptyListBox( parentKey+"_UserList" );

		// Change the button. It will be unnusable and have a different message.
		enableWindow( parentKey+"_Connect", false );
		setText( parentKey+"_Connect", "Disonnecting...");

		// We are disconnecting, we don't need the ChatWindow* map.
		DeleteAllChatWindows();
	}

	void UserListWindow::disconnectedWindowSettings( const std::string& parentKey )
	{
		// Move the parent key around to the center
		setMove( parentKey, disconnectedPosX, disconnectedPosY,
			disconnectedWidth, disconnectedHeight );
		// Move the internal layout of the child windows as well.
		setMove( parentKey+"_UsernameTitle", 10, 10, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_Username", 10, 30, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_IPAddressTitle", 10, 60, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_IPAddress", 10, 80, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_PortTitle", 10, 110, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_Port", 10, 130, 0, 0, SWP_NOSIZE );
		setMove( parentKey+"_Connect", 10, 165, 0, 0, SWP_NOSIZE );

		// Hide the list box and list box title.
		showHide( parentKey+"_UserListTitle", SW_HIDE );
		showHide( parentKey+"_UserList", SW_HIDE );

		// Re-enable the text boxes so they may be editable.
		enableWindow( parentKey + "_Username", true );
		enableWindow( parentKey + "_IPAddress", true );
		enableWindow( parentKey + "_Port", true );
		// Re-enable the button.
		enableWindow( parentKey + "_Connect", true );
		// Reset the text on the button.
		setText( parentKey + "_Connect", "Connect" );
	}

	void UserListWindow::EnableChatWindow( const std::string& name, bool enable )
	{
		// If a chat window exists between this user and the user disconnecting, then we disable the user from being able to continue sending messages.
		if( _hWndManager->isMember(name) )
		{
			BOOL ENABLE = (enable)? TRUE: FALSE;
			//Sends a message to a particular chat window.
			HWND chatHWND = getHWND( name );
			SendMessage( chatHWND, JBIM_CHATENABLE, NULL, ENABLE );
		}
	}

	void UserListWindow::errorLock( const HWND& parent, const std::string& parentKey,
		const std::string& errorMess, const std::string errorHeader )
	{
		// Disable the buttons.
		enableWindow( parentKey+"_Connect", false );

		Error::PopUp( errorMess.c_str(), errorHeader.c_str() );
		PostMessage( parent, JBIM_DISCONNECT, NULL, NULL );
	}

	DWORD WINAPI UserListWindow::realThread( void* parameter )
	{
		//Casts the member back into the hWnd of the parent.
		static const HWND parent = reinterpret_cast<HWND>(parameter);

		//The name of the parent window.
		static const std::string parentKey = getKey( parent );
		char buffer[Constants::dataPackSize];

		//Starts unblocking of the socket.
		_socket->Unblock();


		try
		{
			while( _connected )
			{
				// Receive data.
				int bytesRead = _socket->Receive( buffer, Constants::dataPackSize );

				//If there was no data (or a would block), continue.
				if(!bytesRead) continue;

				//convert the buffer into a data pack.
				DataPack packReceived( buffer );

				std::string header = packReceived.header;
				//Adds the users already connected or just connected to this user's list box.
				if( header == "AddUserToList" )
				{

					//Get the user names from the data pack, convert to a vector of strings
					Users::Vector userNames = Serializer::SillyRabbit<std::string>(packReceived.data, packReceived.size/sizeof(std::string) );

					Users::VectorIterator iter = userNames.begin();
					//iterate through every member in the vector passed.
					while( iter != userNames.end() )
					{
						if( *iter != "" )
						{
							//The entry to this user's list box.
							addListBoxEntry( parentKey+"_UserList", *iter );
							//If there is already a conversation window open between this user any of the others
							// that just joined, re enable it.
							EnableChatWindow( *iter, true);
						}

						++iter;
					}
				}
				else if( header == "RemoveFromList" )
				{
					//convert from stream to string: its the user to remove from the list.
					std::string username(packReceived.data);

					//get the index of the entri that matches
					int index = findListBoxEntry( parentKey+"_UserList", username );
					//remove the entry at the index.
					removeListBoxEntry( parentKey+"_UserList", index );

					//If there is a conversation window open between this user and the one disconnecting,
					//disable this user from sending anything.
					EnableChatWindow( username, false);
				}
				else if( header == "Used Name!!" )
				{
					errorLock( parent, parentKey, packReceived.data, packReceived.header );
				}
				else if( header == "AddToChatLog" )
				{
					//Takes the stream and makes it into a messagepack.
					MessagePack messagePack( packReceived.data );

					//Make the chat window if it already isn't made.
					SendMessage( parent, JBIM_CHATOPEN, NULL, (LPARAM)&messagePack );

					//adds the string sent to the chat window open.
					_chatWindows[messagePack.From]->addToChatLog( messagePack.From, messagePack.Message );
				}

			}

		}
		catch( const WSAException& error )
		{
			// Show any errors that may occur.
			errorLock( parent, parentKey, error.what(), "Client Receive Error!" );
		}

		//Start blocking the socket again.
		_socket->Block();

		return 0;
	}

	DWORD WINAPI UserListWindow::DummyThread( void* parameter )
	{
		// Take the parameter passed and convert it to the Hwnd of the parent.
		HWND hWnd = static_cast<HWND>(parameter);

		Registry& registry = Registry::GetRegistry();
		// Gets the pointer to the class attached to the back of the window object.
		UserListWindow* iWindow = reinterpret_cast<UserListWindow*>( registry.GetWindow(hWnd) );

		// If the pointer has yet to exist, recur until it does.
		if(!iWindow)
			DummyThread(hWnd);
		else
		{
			//Call the interal thread.
			iWindow->realThread(hWnd);
		}

		return 0;
	}

} // end of namespace.
