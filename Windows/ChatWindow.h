#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <vector> //for the user vector.

#include "../Sockets/Socket.h" //Carries all the functionality of a socket in a wraparound class.
#undef _UNICODE
#undef UNICODE
#include <windows.h> // Win32 API.
#include "WinClassEx.h" // For registering a new class template.
#include "HWNDManager.h" // Keeps track of all the instances of windows by IDs.
#include "AbstractWindow.h" // Base class.
#include "../Utilities/ErrorBox.h" // Display errors caught.
#include "../Utilities/MessagePack.h" // for messages
#include "../Resources/resource.h" // For my custom icon.

namespace MyWindows
{
	class ChatWindow :public AbstractWindow
	{
	private:
		//Has a template that needs registering in ctor.
		WinClassEx _winClassEx;

		//ID of the user list window
		const std::string _parentKey;
		//name of the end-user
		const std::string _localUser;
		//name of remote end user
		const std::string _remoteUser;
		//chat log
		std::string _chatLog;

		Socket* _socket;

		//Defines the procedure of this window. Can use any class members.
		LRESULT CALLBACK Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	public:
		//Accepts a hWndManager, the client socket pointer, handle to this process, the name of the parent window that created it, local user name, remote username
		ChatWindow( HWNDManager* hWndManager, HINSTANCE hInstance, Socket* socket, const std::string& parentKey, const std::string& localUser, const std::string& remoteUser );
		//Destructor.
		~ChatWindow( void );
		//Creates a new instance of this window. window name not used, Will be named after the remote user connected to.
		void Instantiate( const std::string& windowName, const unsigned& posX=CW_USEDEFAULT, const unsigned& posY=CW_USEDEFAULT );
		//Will make the window visible. 
		void Show( const std::string& windowName, int nShowCmd );
		//Will Update the window.
		void Update( const std::string& windowName );

		//Adds a string to the chat window. 
		void addToChatLog( const std::string& author, const std::string& message );
	};
}

#endif