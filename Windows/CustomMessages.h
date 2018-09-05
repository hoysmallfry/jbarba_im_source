#ifndef CUSTOMMESSAGES_H
#define CUSTOMMESSAGES_H

//defines custom messages for my windows procedures

//Tells the chat window to either enable or disable the message box if someone leaves
enum
{
	JBIM_CHATENABLE = WM_USER,
	//Tells the user list window to throw away a hWnd of the chat window that was destroyed.
	JBIM_CHATCLOSE,
	//Tells the user list window to create a new window and add it to the hWndManager
	JBIM_CHATOPEN,
	JBIM_SEND,
	JBIM_DISCONNECT,
};


#endif