#include "WSAException.h"

const char* WSAException::getErrorCode( int errorCode )
{

	switch( errorCode )
	{
		case 0:
			return "WinSock Succesful."; break;
		case WSA_INVALID_HANDLE:
			return "Specified event handle object is invalid."; break;
		case WSA_NOT_ENOUGH_MEMORY:
			return "Insufficient memory available."; break;
		case WSA_INVALID_PARAMETER:
			return "One or more parameters are invalid."; break;
		case WSA_OPERATION_ABORTED: 
			return "Overlapped operation canceled due to closure of a socket, or SIO_FLUSH command in WSAIoctl."; break;
		case WSA_IO_INCOMPLETE:
			return "Overlapped I/O event object not in signaled state. Tried to determine status which is not yet completed."; break;
		case WSA_IO_PENDING:
			return "Overlapped operations can not be completed immediately."; break;
		case WSAEINTR:
			return "Interrupted block by WSACancelBlockingCall"; break;
		case WSAEBADF:
			return "The file handle supplied is not valid."; break;
		case WSAEACCES:
			return "An attempt was made to access a socket in a way forbidden by its access permissions."; break;
		case WSAEFAULT:
			return "Detected an invalid pointer value."; break;
		case WSAEINVAL:
			return "Invalid arguement."; break;
		case WSAEMFILE:
			return "Too many open sockets."; break;
		case WSAEWOULDBLOCK:
			return "The resource is temporarily busy."; break;
		case WSAEINPROGRESS:
			return "A blocking operation is currently executing."; break;
		case WSAEALREADY:
			return "Operation attempted on a socket with similar operation already in progress."; break;
		case WSAENOTSOCK:
			return "Operation attempted on something that is not a socket."; break;
		case WSAEDESTADDRREQ:
			return "A destination address is required."; break;
		case WSAEMSGSIZE:
			return "Message sent on a datagram socket is too long."; break;
		case WSAEPROTOTYPE:
			return "Protocol specified in function call that does not support socket type requested."; break;
		case WSAENOPROTOOPT:
			return "Bad protocol option."; break;
		case WSAEPROTONOSUPPORT:
			return "Protocol not supported."; break;
		case WSAESOCKTNOSUPPORT:
			return "Socket type not supported."; break;
		case WSAEOPNOTSUPP: 
			return "Operation not supported."; break;
		case WSAEPFNOSUPPORT: 
			return "Protocol family not supported."; break;
		case WSAEAFNOSUPPORT: 
			return "Address family not supported by protocol family."; break;
		case WSAEADDRINUSE: 
			return "Address already in use. Are you binding twice?"; break;
		case WSAEADDRNOTAVAIL: 
			return "Cannot assign the requested address in it's context."; break;
		case WSAENETDOWN:
			return "The network is down."; break;
		case WSAENETUNREACH:
			return "The network is currently unreachable."; break;
		case WSAENETRESET: 
			return "the network dropped connection on reset."; break;
		case WSAECONNABORTED: 
			return "Software caused a connection abort."; break;
		case WSAECONNRESET: 
			return "Connection reset by peer."; break;
		case WSAENOBUFS: 
			return "No buffer space is available."; break;
		case WSAEISCONN: 
			return "Socket is already connected."; break;
		case WSAENOTCONN: 
			return "Socket is not connected."; break;
		case WSAESHUTDOWN: 
			return "Attempting to send data after socket shutdown."; break;
		case WSAETOOMANYREFS:
			return "Too many references to some kernel object."; break;
		case WSAETIMEDOUT: 
			return "Connection timed out."; break;
		case WSAECONNREFUSED: 
			return "Connection actively refused by target."; break;
		case WSAELOOP: 
			return "Cannot translate name."; break;
		case WSAENAMETOOLONG: 
			return "Name too long."; break;
		case WSAEHOSTDOWN: 
			return "Socket operation failed, destination host is down."; break;
		case WSAEHOSTUNREACH: 
			return "No route to host available."; break;
		case WSAENOTEMPTY: 
			return "Cannot remove a directory that is not empty."; break;
		case WSAEPROCLIM: 
			return "Too many processes."; break;
		case WSAEUSERS: 
			return "User quota has exceeded."; break;
		case WSAEDQUOT: 
			return "Disk quota exceeded."; break;
		case WSAESTALE: 
			return "File handle reference is no longer available."; break;
		case WSAEREMOTE: 
			return "Item is not available locally."; break;
		case WSASYSNOTREADY: 
			return "Network subsystem is unavailable"; break;
		case WSAVERNOTSUPPORTED: 
			return "Winsock.dll version out of range."; break;
		case WSANOTINITIALISED: 
			return "Successful WSAStartup not yet performed."; break;
		case WSAEDISCON: 
			return "Graceful shutdown in progress."; break;
		case WSAENOMORE: 
			return "No more results."; break;
		case WSAECANCELLED:
			return "Call has been cancelled."; break;
		case WSAEINVALIDPROCTABLE:
			return "Service provider procedure call table is invalid."; break;
		case WSAEINVALIDPROVIDER: 
			return "Service provider is invalid."; break;
		case WSAEPROVIDERFAILEDINIT:
			return "Service provider failed to initialize."; break;
		case WSASYSCALLFAILURE:
			return "System call failure."; break;
		case WSASERVICE_NOT_FOUND:
			return "No such service found."; break;
		case WSATYPE_NOT_FOUND: 
			return "Specified class was not found."; break;
		case WSA_E_NO_MORE: 
			return "No more results."; break;
		case WSA_E_CANCELLED:
			return "Call was canceled."; break;
		case WSAEREFUSED: 
			return "Database query was refused."; break;
		case WSAHOST_NOT_FOUND: 
			return "Host not found."; break;
		case WSATRY_AGAIN: 
			return "Non-authoritative host not found."; break;
		case WSANO_RECOVERY:
			return "Irrecoverable error."; break;
		case WSANO_DATA:
			return "Valid name, no data record of requested type."; break;
		case WSA_QOS_RECEIVERS:
			return "At least one QOS reserve has arrived."; break;
		case WSA_QOS_SENDERS:
			return "At least one QOS send path has arrived."; break;
		case WSA_QOS_NO_SENDERS:
			return "There are no QOS senders."; break;
		case WSA_QOS_NO_RECEIVERS:
			return "There are no QOS receivers."; break;
		case WSA_QOS_REQUEST_CONFIRMED:
			return "QOS request confirmed."; break;
		case WSA_QOS_ADMISSION_FAILURE:
			return "QOS admission error."; break;
		case WSA_QOS_POLICY_FAILURE:
			return "QOS policy error."; break;
		default:
			return "Unknown error."; break;
	}

	return "";
}

//Gets the message from the error code.
WSAException::WSAException( const int& errorNumber )
: _error( getErrorCode(errorNumber) ) {}

//Returns the message.
const char*  WSAException::what( void ) const
{
	return _error;
}
