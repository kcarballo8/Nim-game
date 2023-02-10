#include "Nim.h"
#include <WinSock2.h>
#include <iostream>
#include <string>

// getServers.cpp
//  Given a socket, a broadcast address and a port number, this function returns the number
//  of servers as well as an array of structs.  Each struct contains the name, IP_Address and 
//  port number of a remote server.

//	IN parameters:
//	SOCKET s				= Allocate UDP socket handle that can be used for communcations
//	char *broadcastAddress	= The broadcast address that should be used for current LAN
//	char *broadcastPort		= The port that should be used for the broadcast

//	OUT parameters:
//	ServerStruct server[]	= An array that contains the structs.  The members of each struct
//							  contain the name, IP Address and port number of remote server.
//	numServers				= Number of elements in the server[] array

//	Function return value	= Number of remote servers (size of server[] array)

void getServers(SOCKET s, char* broadcastAddress, char* broadcastPort, ServerStruct server[], int& numServers)
{
	std::string host;
	std::string port;

	numServers = 0;

	// Send TicTacToe_QUERY to broadcastAddress using broadcastPort
/*****
	Task 3: Add code here that will send the TicTacToe_QUERY message to the broadcastAddress
			using the broadcastPort (see function header)
*****/

	char buf[MAX_SEND_BUF];

	char send[MAX_SEND_BUF];
	strcpy_s(send, NIM_QUERY);
	UDP_send(s, send, (strlen(NIM_QUERY)), broadcastAddress, broadcastPort);


	// Receive incoming UDP datagrams (with a maximum of 2 second wait before each recv() function call)
	// As you read datagrams, if they start with the prefix: TicTacToe_NAME, parse out the server's name
	// and add the name, host address and port number to the server[] array.  Don't forget to increment numServers.
	int status = wait(s, 2, 0);
	if (status > 0) {
		int len = 0;

/*****
	Task 4a: Add code here that will receive a response to the broadcast message
*****/

		len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());

		while (status > 0 && len > 0) {

/*****
	Task 4b: Inside this while loop, extract a response which should be a C-string that
			 looks like "Name=some server's name"
			 If the response doesn't begin with the characters "Name=", ignore it.
			 If it does begin with the characters "Name=", parse the actual name that follows and
			   (i) assign that name to the array of structs: server[numServers].name
			  (ii) assign the IP address from which the response originated to serverArray[numServers].host
			 (iii) assign the server's port number to serverArray[numServers].port
			  (iv) increment numServers
*****/
			char* add;
			add = strstr(buf, "Name=");
			if (add != nullptr) {
				server[numServers].name = buf;
				server[numServers].host = host.c_str();
				server[numServers].port = port.c_str();
				numServers++;
			}
			
			status = wait(s, 2, 0);
			if (status > 0)
				len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
		}
	}
}
