/*------------------------------------------------------------
 * UDP_recv - Receive a string from a host machine using UDP
 *------------------------------------------------------------*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#define MAX_IP_ADDRESS_LEN 16
#define MAX_PORT_LEN 6

int UDP_recv(SOCKET s, char* buf, int maxsize, char* host, char* port)
{	// Parameter List:
	// [IN] s      : SOCKET handle
	// [OUT] buf   : Address of NULL-terminated char array that will contain the data received.
	// [IN] maxsize: Integer containing the maximum number of bytes that buf can hold
	// [OUT] host  : Address of NULL-terminated char array containing IP Address of machine that sent the data
	// [OUT] port  : Address of NULL-terminated char array containing port number on remote host that sent the data
	// Return Value: Number of bytes actually received.

	// NOTES: 1. This function requires WinSock2.h and iostream header files
	//             as well as the ws2_32.lib library file.
	//        2. This function will block until data is received from some other machine.

	sockaddr_in sin;						/* an Intenet endpoint address */
	int         sin_size = sizeof(sin);		/* Needed for sixth parameter in recvfrom function call */
	int         len;						/* Holds return value for recvfrom function call */

	if (s < 0 || s == INVALID_SOCKET) {
		std::cout << "Problem with UDP_recv().  Invalid socket handle." << std::endl;
		len = -1;
	}
	else {
		memset(&sin, 0, sizeof(sin));				/* zero the sockaddr_in struct */

		/* Wait for and read incoming data from ANY remote host/port */
		len = recvfrom(s, buf, maxsize, 0, (LPSOCKADDR)&sin, &sin_size);
		if (len <= 0 || len > maxsize) {
			std::cout << std::endl << "Problem with recvfrom() function call within UDP_recv().  Error Code = " << WSAGetLastError() << std::endl;
			std::cout << "Length = " << len << std::endl;
		}
		else {
			// Find the sender's IP address and port #
			strcpy_s(host, MAX_IP_ADDRESS_LEN, inet_ntoa(sin.sin_addr));
			_itoa_s(ntohs(sin.sin_port), port, MAX_PORT_LEN, 10);

			// Make sure the characters in buf[] are null-terminated
			if (buf[len - 1] != '\0') {
				if (len < maxsize)
					buf[len] = '\0';
				else
					buf[maxsize - 1] = '\0';
			}
		}
	}
	return len;
}
