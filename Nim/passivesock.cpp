/*----------------------------------------------------------------
 * passivesock - allocate & bind a server socket using TCP or UDP
 *---------------------------------------------------------------*/
#ifndef	WINSOCK2
#define	WINSOCK2
#include <winsock2.h>
#pragma	comment	(lib, "ws2_32.lib")
#endif

#include <iostream>

SOCKET passivesock(char* service, char* protocol) {
	// Parameter List:
	// [IN] service : String containing service type as name (eg. ECHO) or port number (eg. 7)
	// [IN] protocol: String containing protocol type (either "tcp" or "udp")
	// Return value : SOCKET handle of server-type socket.

	//	NOTE: This function requires winsock2.h header file and ws2_32.lib library file.

	servent* pse;		/* pointer to service information entry   */
	sockaddr_in  server;	/* an Internet endpoint address           */
	SOCKET       s;			/* socket descriptor                      */

	/* Use protocol to set type and allocate socket */
	if (_stricmp(protocol, "tcp") == 0)
		s = socket(AF_INET, SOCK_STREAM, 0);
	else if (_stricmp(protocol, "udp") == 0)
		s = socket(AF_INET, SOCK_DGRAM, 0);
	else
		s = INVALID_SOCKET;
	if (s < 0 || s == INVALID_SOCKET) {
		std::cout << "Cannot create socket" << std::endl;
		return INVALID_SOCKET;
	}

	memset(&server, 0, sizeof(server));		/* zero the sockaddr_in struct */

	server.sin_family = AF_INET;			/* Set the address family to AF_INET (= Internet) */
	server.sin_addr.s_addr = INADDR_ANY;	/* Set IP Address to INADDR_ANY constant */

	/* Map service name (or number) to port number */
	if (pse = getservbyname(service, protocol))
		server.sin_port = (u_short)pse->s_port;
	else if ((server.sin_port = htons((u_short)atoi(service))) == 0) {
		std::cout << "Invalid service request" << std::endl;
		return INVALID_SOCKET;
	}

	/* Bind the socket */
	if (bind(s, (LPSOCKADDR)&server, sizeof(SOCKADDR)) < 0) {
		std::cout << "Cannot bind socket to port: " << service << std::endl;
		return INVALID_SOCKET;
	}

	if (_stricmp(protocol, "tcp") == 0)
		if (listen(s, 5) < 0) {
			std::cout << "Cannot listen on port: " << service << std::endl;
			return INVALID_SOCKET;
		}

	return s;
}
