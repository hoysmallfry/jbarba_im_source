#ifndef USERLISTWINDOW_H
#define USERLISTWINDOW_H

#include <string>
#include <map> //for the user vector.

#include "../Utilities/Users.h"
#include "../Utilities/SqueakyCleanWindows.h"
#include "../Utilities/ErrorBox.h" // Display errors caught.

#include "../Sockets/Socket.h" //Carries all the functionality of a socket in a wraparound class.

#include "WinClassEx.h"
#include "HWNDManager.h" // Keeps track of all the instances of windows by IDs
#include "AbstractWindow.h"
#include "ChatWindow.h" // Used for the chat window map.

#include "../Threads/AbstractThread.h"
#include "../Threads/ThreadManager.h" // Creates threads and keeps track of all the handles.

#include "../Resources/resource.h" // For my custom icon.

namespace MyWindows
{
	class UserListWindow
		:public AbstractWindow, public Threads::AbstractThread//, public Users
	{
	private:

		// DEfines a map of windows objects.
		typedef std::map< std::string, ChatWindow* > ChatWindowMap;
		// definition of iterator for the map.
		typedef ChatWindowMap::iterator ChatWindowIterator;

		// Elements will exist upon first creation unto disconnection.
		ChatWindowMap _chatWindows;

		// Class for the user list window itself.
		WinClassEx _winClassEx;

		// Window will have different settings/layout based on connection.
		bool _connected;

		Socket* _socket;

		// Helper functions:
		// Returns true if the specified edit box is empty.
		bool editBoxEmpty( const std::string& windowName );
		// Checks to see if all three are empty. Returns false if all are not empty.
		bool editBoxesEmpty( const std::string& parent );

		// Creates a new socket and attempts to connect.
		void connectSocket( sockaddr_in remote );
		// Sends the server a 'user disconnect' message and shuts down.
		void disconnectSocket( const std::string& parentKey  );

		// Changes the layout of the window depending what modes its in.
		void connectingWindowSettings( const std::string& parentKey );
		void connectedWindowSettings( const std::string& parentKey );
		void disconnectingWindowSettings( const std::string& parentKey );
		void disconnectedWindowSettings( const std::string& parentKey );

		void MakeChatWindow( const HWND& hWnd, const LPARAM& lParam );
		// Creates a new windows object if needed, a new instance of the window if needed.
		void MakeChatWindow( const std::string& parentKey, const std::string& localUser, const std::string& remoteUser );
		// Destroys an instance of a registered window, removes from hWndManager.
		void DestroyChatWindow( HWND hWnd );
		// Removes all the open chat windows and removes them from hWndManager.
		void DestroyAllChatWindows( void );
		// Completely deletes all windows objects from the ChatWindowMap.
		void DeleteAllChatWindows( void );
		// Informs a particular chat window to either disable/enable itself if a user leaves/comes back.
		void EnableChatWindow( const std::string& name, bool enable );

		// The internal thread that can use class members.
		DWORD WINAPI realThread( void* parameter );

		//Defines the internal procedure of this window.
		LRESULT CALLBACK Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
		void handleChildMessages( const HWND& hWnd, const WPARAM& wParam, const LPARAM& LPARAM );
		void handleEditBoxMessages( const HWND& hWnd, const LPARAM& lParam );
		void handleListBoxMessages( const HWND& hWnd, const LPARAM& lParam );
		void handleButtonMessages( const HWND& hWnd, const LPARAM& lParam );
		void connect( const HWND& hWnd, const std::string& parentKey );
		void disconnect( const HWND& hWnd );
		void sendServerMessage( const HWND& hWnd );
		void endProgram( const HWND& hWnd );

		void errorLock( const HWND& parent, const std::string& parentKey,
			const std::string& errorMess, const std::string errorHeader );

	public:
		// Assignment constructor. Accepts all the needed external entities. 
		UserListWindow( Threads::ThreadManager* threadManager, HWNDManager* hWndManager, HINSTANCE hInstance, Socket* socket );
		// Destructor. Deletes members of ChatWindowMap.
		~UserListWindow( void );

		// Creates a new instance of the window.
		void Instantiate( const std::string& windowName, const unsigned& posX=CW_USEDEFAULT, const unsigned& posY=CW_USEDEFAULT );
		// For showing the window.
		void Show( const std::string& windowName, int nShowCmd );
		// Updates the window.
		void Update( const std::string& windowName );
		
		//Refines the external thread function.
		static DWORD WINAPI DummyThread( void* parameter );
	};
}

#endif