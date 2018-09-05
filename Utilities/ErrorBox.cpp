#include "ErrorBox.h"

#include "SqueakyCleanWindows.h"
#include <tchar.h> //enable use of TCHARs
#include <string> //for the shell function

void Error::PopUp( const char* error, const char* header )
{
	//Message box with no parent, displays winsock error, with header "WinSockError!"
	//and uses the error style with red X icon.
	MessageBox( NULL, _T(error), _T(header), MB_ICONERROR );
}
