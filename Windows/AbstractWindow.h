// ---------------------------------------------------------------------------
// Project Name		:	Simple Framework
// File Name		:	AbstractWindow.h
// Author			:	Joel Barba
// Creation Date	:	2009/10/11
// Purpose			:	header file for a generic window.
// ---------------------------------------------------------------------------

#ifndef ABSTRACTWINDOW_H
#define ABSTRACTWINDOW_H

#include <map>

#include "../Utilities/SqueakyCleanWindows.h" //windows lite

#include "HWNDManager.h" //Keeps track of all the instances of windows by IDs
#include "CustomMessages.h"

namespace MyWindows
{
	class AbstractWindow
	{
	// Protected so it may pass on to any derived classes.
	protected:
		//handle to process
		HINSTANCE _hInstance;
		//The window handler manager
		HWNDManager* _hWndManager;

		// Creates the parent window defined by WinClassEx in derived classes.
		void CreateParentWindow( const std::string& className, const std::string& name, DWORD style, int x, int y, int width,int height );
		// Will create child window with the parent defined by the parent created from the above function.
		void CreateChildWindow( const std::string& type, const std::string& parentName, const std::string& postfix, DWORD style, int x, int y, int width, int height, const std::string& string = "" );
		
		// Will get the name of a window from the hWndManager based on its value.
		const std::string& getKey( const HWND& hWnd ) const;
		// Will get the hWnd from the manager based on its name.
		const HWND getHWND( const std::string& name ) const;
		// Will remove the hWnd from the manager based on its name.
		void removeHWND( const std::string& name );

		//Generic window functions:
		//Creates a custom icon with a icon value found resources.h
		HICON CustomIcon( int iconValue );
		// Will make a particular window under name within the manager visible or invisible. 
		void showHide( const std::string& name, UINT flags );
		//Will resize or reposition a window under name in the hWndManager.
		void setMove( const std::string& name, int x, int y, int width, int height, UINT flags=NULL );
		//Will resize or reposition a window under name in the hWndManager.
		RECT getMove( const std::string& name ) const;
		//Sets the text an window in hWndManager with key name with text.
		void setText( const std::string& name, const std::string& text );
		// Like the function above but replaces text with "".
		void clearText( const std::string& name );
		// Gets the length of the text in the window in hWndManager with key name.
		unsigned getTextLength( const std::string& name ) const;
		// Gets the text in the window in hWndManager with key name.
		std::string getText( const std::string& name ) const;
		// Enables the edit box in hWndManager with key name.
		void enableWindow( const std::string& name, bool enabled );

		// Edit Box functions:
		// Limits the maximum text in edit box in hWndManager with key name.
		void limitEditText( const std::string& name, const unsigned int size );
		void scrollEditText( const std::string& name, const int vertical=0, const int horizontal=0 );
		unsigned getEditLineCount( const std::string& name );


		// List Box functions:
		// Adds an entry to the list box in hWndManager with key name.
		void addListBoxEntry( const std::string& name, const std::string& entry );
		// Removes an entry at index from the list box in hWndManager with key name.
		int removeListBoxEntry( const std::string& name, int index );
		// Clears all entries in the list box in hWndManager with key name.
		void emptyListBox( const std::string& name );
		// Returns the index of first entry comparible to parameter 2 in list box in hWndManager with key name.
		int findListBoxEntry( const std::string& name, const std::string& comparison );
		// Returns the contents of the entry at index of the list box in hWndManager with key name.
		std::string getListBoxEntry( const std::string& name, int index ) const;
		// Returns the index of the entry highlighted of the list box in hWmdManager with key name.
		int getListBoxCursorSelection( const std::string& name ) const;

		//Defines the procedure of this window. Purely abstract.
		virtual LRESULT CALLBACK Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) = 0;

	public:
		// Assignment Constructor. Assigns pointers to native members.
		AbstractWindow( HWNDManager* hWndManager, HINSTANCE hInstance );
		//Destructor. Can be overloaded in cases where Liskov Substitution is used.
		virtual ~AbstractWindow( void ){}

		//Shows the window. Purely abstract.
		virtual void Show( const std::string& windowName, int nShowCmd );
		//Updates the window. Purely abstract.
		virtual void Update( const std::string& windowName );
		//Creates an instance of a class. Purely abstract.
		virtual void Instantiate( const std::string& windowName, const unsigned& posX=CW_USEDEFAULT, const unsigned& posY=CW_USEDEFAULT ) = 0;

		static void halt_windows(void);

		// External procedure assigned to the WinClassEx template. Will call the external proc to use members.
		static LRESULT CALLBACK DummyProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	};

	class Registry
	{
	public:
		static Registry& GetRegistry( void )
		{
			static Registry singletonRegistry;
			return singletonRegistry;
		}

		void Register( HWND hWnd, AbstractWindow* window )
		{
			windows[hWnd] = window;
		}
		AbstractWindow* GetWindow( HWND hWnd )
		{
			window_map::iterator iter = windows.find(hWnd);

			if( iter == windows.end() )
				return 0;

			return iter->second;
		}
	private:
		Registry( void ){}

		//you should wrap this into a singleton registry
		typedef std::map<HWND, AbstractWindow*> window_map;
		window_map windows;
	};
}

#define THROW_T(x) if (true) { MyWindows::AbstractWindow::halt_windows(); throw std::runtime_error(x); } else (void)0

#endif
