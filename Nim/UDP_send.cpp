/*------------------------------------------------------------
 * UDP_send - Send a string to a host machine using UDP
 *------------------------------------------------------------*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>

int UDP_send(SOCKET s, char* buf, int numBytesToSend, char* host, char* service)
{	// Parameter List:
	// [IN] s      : SOCKET handle
	// [IN] buf    : Address of NULL-terminated char array containing bytes to be sent to host
	// [IN] numBytesToSend: Integer containing the number of bytes in buf that should be sent
	//                       Caution: This integer should be <= the size of the buf char array!
	// [IN] host   : Address of NULL-terminated char array containing domain name of remote host or IP Address
	//               Broadcast-type IP Address (such as 10.1.255.255) is supported.
	// [IN] service: Address of NULL-terminated char array containing service name or port number to use for transmission
	// Return Value: Number of bytes actually sent.

	//	NOTE: This function requires the <iostream> and WinSock2.h header files
	//        as well as the ws2_32.lib library file.

	struct hostent* phe;   /* pointer to host information entry     */
	struct servent* pse;	/* pointer to service information entry  */
	sockaddr_in		sin;	/* an Intenet endpoint address           */
	long			ihost;	/* 32-bit IP address number              */
	int				len;	/* Number of bytes actually sent         */

	if (s < 0 || s == INVALID_SOCKET) {
		std::cout << "Problem with UDP_send().  Invalid socket handle." << std::endl;
		len = -1;
	}
	else {
		memset(&sin, 0, sizeof(sin));  /* zero the sockaddr_in struct */

		/* Set the address family to AF_INET (= Internet) */
		sin.sin_family = AF_INET;

		/* Map service name (or number) to port number */
		if (pse = getservbyname(service, "UDP"))
			sin.sin_port = pse->s_port;
		else if ((sin.sin_port = htons((u_short)atoi(service))) == 0) {
			std::cout << "Invalid service request." << std::endl;
			return INVALID_SOCKET;
		}

		/* Map host name to IP address, allowing for dotted quad */
		ihost = inet_addr(host);
		if (ihost == INADDR_NONE) {
			phe = gethostbyname(host);
			if (phe)
				memmove(&ihost, phe->h_addr, phe->h_length);
			else {
				std::cout << "Invalid host" << std::endl;
				return INVALID_SOCKET;
			}
		}
		sin.sin_addr.s_addr = ihost;

		len = sendto(s, buf, numBytesToSend, 0, (LPSOCKADDR)&sin, sizeof(sin));
		if (len <= 0) {
			std::cout << std::endl << "Problem with sendto() function call within UDP_send().  Error Code = " << WSAGetLastError() << std::endl;
		}
	}

	return len;
}