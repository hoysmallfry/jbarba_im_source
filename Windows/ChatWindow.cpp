#include "ChatWindow.h"
#include "../Utilities/Constants.h"
#include "../Utilities/DataPack.h"

#include <math.h>
#include <time.h> //for seeding rand

const unsigned width = 600;
const unsigned height = 500;

const unsigned posX = (Constants::centerX)-(width/2);
const unsigned posY = (Constants::centerY)-(width/2);

namespace MyWindows
{
	//Assignment Constructor, object will know its parent(UserListWindow that spawned it.), the user logged on from this end, the user logged on other end. Chat log starts empty.
	ChatWindow::ChatWindow( HWNDManager* hWndManager, HINSTANCE hInstance, Socket* socket, const std::string& parentKey, const std::string& localUser, const std::string& remoteUser )
		: AbstractWindow( hWndManager, hInstance ), _socket(socket), _parentKey(parentKey), _localUser(localUser), _remoteUser(remoteUser), _chatLog( "" )
	{
		//Uses the wrap around to set up a window template.
		_winClassEx = WinClassEx( "ChatWindow", _hInstance, DummyProcedure, CreateSolidBrush(RGB(0,180,0)),
			CustomIcon(ME_ICON), CustomIcon(ME_ICON) );

		//Register the class. operator () return the WNDCLASSEX member inside.
		RegisterClassEx( &_winClassEx() );
	}
	ChatWindow::~ChatWindow( void )
	{
	}

	void ChatWindow::Instantiate( const std::string& windowName, const unsigned& posX, const unsigned& posY )
	{
		//Create a parent
		CreateParentWindow( _winClassEx().lpszClassName, _remoteUser, WS_SYSMENU, ::posX, ::posY, width, height );

		//Create the children. Think of the children.
		CreateChildWindow( "STATIC", _remoteUser, "ChatLogTitle", NULL, 10, 10, 570, 20, "Chat Log:" );
		CreateChildWindow( "EDIT", _remoteUser, "ChatLog", WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY, 10, 10, 570, 300 );
		
		CreateChildWindow( "EDIT", _remoteUser, "Message", WS_BORDER | WS_VSCROLL | ES_MULTILINE, 10, 340, 450, 100, "" );
		CreateChildWindow( "BUTTON", _remoteUser, "Send", WS_BORDER | BS_DEFPUSHBUTTON, 480, 340, 100, 100, "SEND" );
		
		//limits the size of messages sent.
		limitEditText( _remoteUser+"_Message", Constants::messageSize-1 );

		//if there is something in the _chatLog string add it to the chat log.
		setText( _remoteUser+"_ChatLog", _chatLog );
	}

	void ChatWindow::Show( const std::string& windowName, int nShowCmd )
	{
		// Gets handle based on name.
		HWND hWnd = getHWND( _remoteUser );
		// Shows particular window.
		ShowWindow( hWnd, nShowCmd );
	}

	void ChatWindow::Update( const std::string& windowName )
	{
		// Gets handle based on name.
		HWND hWnd = getHWND( _remoteUser );
		// Update particular window.
		UpdateWindow( hWnd );
	}

	LRESULT CALLBACK ChatWindow::Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		//The structure that used when painting
		PAINTSTRUCT ps;
		HDC hdc;
	
    	switch (message)
		{
		case WM_COMMAND:
			{

				std::string parentKey = getKey(hWnd);
				//the hiword is the command
				//the loword is the id
				//and lparam is the hwnd
				switch( HIWORD(wParam) )
				{
				case EN_CHANGE:
					{
						std::string messageName = parentKey+"_Message";
						std::string message = getText( messageName );
						size_t lessLength = message.length() - 1;
						char lastChar = *( message.c_str() + lessLength );

						//If enter was pressed, simulate a the send being pushed
						if( lastChar == '\n' )
						{
							message.resize( lessLength );
							setText( messageName, message);
							SendMessage( hWnd, JBIM_SEND, NULL, NULL );
						}
					}
					break;

				case BN_CLICKED:
					{	
						SendMessage( hWnd, JBIM_SEND, NULL, NULL );
					}
					break;
				}
			}
			break;
		case JBIM_SEND:
			{
				std::string messageName = _remoteUser+"_Message";
				//Get the string from the edit box.
				std::string text = getText( messageName );

				//Clear the edit box
				clearText( messageName );

				//Adds to the chat log
				addToChatLog( _localUser, text );

				//Creates a message pack to send to the remote user
				MessagePack messagePack( _remoteUser.c_str(), _localUser.c_str(), text.c_str(), static_cast<unsigned int>(text.length())+1 );

				//Put the message pack in a data pack.
				DataPack dataPack( "AddToChatLog", &messagePack, sizeof(messagePack) );

				//sends the datapack to the server which routes it to the remote user.
				_socket->Send( &dataPack, sizeof(dataPack) );
			}
			break;

		case JBIM_CHATENABLE:
			{
				// Gets the parent key from the hWnd passed
				std::string parentKey = getKey(hWnd);
				// Enables or disables the edit box and button based on the value of lParam.
				enableWindow( parentKey +"_Message", lParam != 0 );
				enableWindow( parentKey +"_Send", lParam != 0 );
				
				//if being disabled, tell this user the disconnecting user is disconnecting
				if( !lParam )
					addToChatLog( "Server", "USER HAS LEFT CHAT." );
			}
			break;

		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
			}
			break;

		case WM_DESTROY:
			{
				// Remove the child windows from the manager.
				removeHWND( _remoteUser+"_ChatLogTitle" );
				removeHWND( _remoteUser+"_ChatLog" );
				removeHWND( _remoteUser+"_Message" );
				removeHWND( _remoteUser+"_Send" );

				// User list window will handle removal of this window from hWndManager.
				SendMessage( getHWND( _parentKey ), JBIM_CHATCLOSE, 0, (LPARAM)hWnd );
				break;
			}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;

		}

		return 0;
	}

	void ChatWindow::addToChatLog( const std::string& author, const std::string& message )
	{
		//Add to the native log string.
		_chatLog += author + ": " + message + "\r\n";

		std::string chatLogName = _remoteUser+"_ChatLog";

		//Set the text in the chat log box.
		setText( chatLogName, _chatLog );
		// Scroll all the way down.
		scrollEditText( chatLogName, getEditLineCount(chatLogName) );
	}
}
