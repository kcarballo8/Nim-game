#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
using namespace std;

/*------------------------------------------------------------------
 * connectsock - allocate & connect a client socket using TCP or UDP
 *------------------------------------------------------------------
 */
SOCKET connectsock(char* host, char* service, char* protocol)
{	// Parameter List:
	// [IN] host    : String containing remote host as either a DNS name (e.g. "www.harding.edu") or an IP address (e.g. "162.242.214.217")
	// [IN] service : String containing remote service type as name (e.g. "ECHO") or port number (e.g. "7")
	// [IN] protocol: String containing protocol type (either "tcp" or "udp")
	// Return value : SOCKET handle of connected socket.

	//	NOTES: 1. This function requires iostream, winsock.h, and wsock32.lib
	//		   2. The host and service parameters are not used for UDP sockets
	//			  Empty strings may be used if protocol = "UDP"

	hostent* phe;				// pointer to host information entry
	servent* pse;				// pointer to service information entry
	struct sockaddr_in client;	// an Intenet endpoint address
	SOCKET s;					// socket handle
	int status;					// status of connection attempt

	/* Allocate and connect socket - type depends upon protocol */
	if (_stricmp(protocol, "tcp") == 0)
	{
		/* Create a TCP socket */
		s = socket(AF_INET, SOCK_STREAM, 0);
		if (s < 0 || s == INVALID_SOCKET)
		{
			cout << "Cannot create socket. Error code = " << WSAGetLastError() << endl;
			return INVALID_SOCKET;
		}

		memset(&client, 0, sizeof(client));						/* zero the client struct */

		client.sin_family = AF_INET;							/* Set the address family to AF_INET = Internet */

		// Map host name to remote IP address, allowing for dotted quad
		long ihost;						// 32-bit IP address in Network Byte Order (big endian)
		ihost = inet_addr(host);		// 1st try to convert from "dotted decimal notation"
		if (ihost == INADDR_NONE) {		// if that doesn't work, send request to DNS
			phe = gethostbyname(host);
			if (phe != NULL) {			// If phe isn't NULL, DNS returned a matching IP address!
				memmove(&ihost, phe->h_addr, phe->h_length);
			}
			else {
				cout << "Invalid host" << endl;
				return INVALID_SOCKET;
			}
		}

		client.sin_addr.s_addr = ihost;				/* Set remote IP address */

													// Map service name (or number) to port number
		pse = getservbyname(service, protocol);		// Search for matching service name
		if (pse != NULL) {							// If found, retrieve associated port number
			client.sin_port = (u_short)pse->s_port;
		}
		else {										// If not found, treat service as an integer
			short port = atoi(service);
			if (port > 0) {
				client.sin_port = htons(port);
			}
			else {
				cout << "Invalid service request" << endl;
				return INVALID_SOCKET;
			}
		}

		/* Connect the TCP socket */
		status = connect(s, (LPSOCKADDR)&client, sizeof(SOCKADDR));
		if (status == SOCKET_ERROR)
		{
			cout << "Remote host/service not found - or connection refused. Error code = " << WSAGetLastError() << endl;
			return INVALID_SOCKET;
		}
	}
	else if (_stricmp(protocol, "udp") == 0)
	{
		s = socket(AF_INET, SOCK_DGRAM, 0);
		if (s < 0 || s == INVALID_SOCKET)
		{
			cout << "Cannot create socket" << endl;
			return INVALID_SOCKET;
		}
	}
	else
	{
		cout << "Invalid Protocol" << endl;
		return INVALID_SOCKET;
	}

	return s;
}
