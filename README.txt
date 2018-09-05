/***************************************************
Author: Joel Barba
Date: 2-10-2010
Class: CS260
Assignment: 2

***************************************************/

Compiling/Running:
Build using Visual Studio 2008. Open the jbarba_cs260.sln. 
When the Run button is pressed, it sould compile and run both server and client. 
If not, right click on the project in the Solution Explorer and choose 
"Debug->Start new instance".

What it does:

------------- Server -------------
will notify the user of notable actions and data, and prompts for the port number to
listen on. The server is capable of serving multiple clients at an indetermined maximum.
The Server will display a message along with information about the client that sent it.

If the server fails of is closed with the [x] button in the top right corner, the client
programs are notified and forced to lock the user out of input.

------------- Client -------------
Prompts user for an IP address and Port number, and a name. The Connect button
will not activate unless there is text in all fields. When the fields are filled,
press the "Connect" button to connect to the server app. If there are any problems
in connecting or sending, the client will display a pop-up message box displaying
the error, and lock all child windows from recieving input. Such errors include all
winsock errors (See implementation file of the WSAException class). 

When connected to the server, if they are are other clients connected, a list of names
will be displayed. If the user doucle clicks on a user name, a chat window will pop up.
the client will then be able to enter a message and send with either Return key or the "Send"
button. When this message is sent, The both clients will display the message sent with
the name of the author who sent it.

To disconnect, hit the disconnect button. Pressing the close [X] button at the top right
corner of the window will gracefully disconnect from the server and end the program.