/*------------------------------------------------------------------------------
 * wait - Given a socket and number of seconds (and/or milliseconds) watch for
 *        for incoming data on the port until time limit expires.
 *        Returns 1 if data waiting; 0 if time expired before data arrived
 *------------------------------------------------------------------------------*/
#ifndef WINSOCK2
#define WINSOCK2
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")
#endif

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

int wait(SOCKET s, int seconds, int msec) {
	// Parameter List:
	// [IN] s		: SOCKET handle of connected socket.
	// [IN] seconds : Number of seconds to wait for incoming traffic
	// [IN] msec	: Number of milliseconds to wait
	// Return value : 1 = Data is waiting in TCP/UDP buffer; 0 = Timeout or error detected

	// Given a socket and an amount of time (in seconds + milliseconds), this function
	// will check to see if any data is waiting to be read from the socket.  The function
	// will wait for the alloted amount of time.  If no data arrives at the socket within
	// the alloted amount of time, the function will exit and return the value 0.
	// CAUTION:  If the remote machine associated with a TCP socket has closed the connection,
	//           this function may still return 1.  This is because the socket is in the
	//           "close pending" state.  A subsequent call to recv() or recvfrom() will 
	//           immediately return 0 (if connection was closed gracefully) or SOCKET_ERROR
	//           if the connection was aborted.  Thus, it is important to check the return
	//           value of the recv() function that follows a call to this wait() function.

	// Note: This function requires the winsock.h header file and wsock32.lib library.

	int    stat;					// Status of waiting socket
	struct timeval stTimeOut;		// Used to set timeout value
	fd_set stReadFDS;				// "File Descriptor Set" for select() call
	fd_set stXcptFDS;				// "File Descriptor Set" for exception handling

	// Set TimeOut value
	stTimeOut.tv_sec = seconds;	// Number of seconds
	stTimeOut.tv_usec = msec;		//   + msec milliseconds to wait

	FD_ZERO(&stReadFDS);			// Zero the set of "Read access" sockets
	FD_ZERO(&stXcptFDS);			// Zero the set of "Exception" sockets
	FD_SET(s, &stReadFDS);			// Add "s" to the set of "Read access" sockets
	FD_SET(s, &stXcptFDS);			// Add "s" to the set of "Exception" sockets

	// Check to see if Read access is enabled for socket "s"
	stat = select(-1, &stReadFDS, NULL, &stXcptFDS, &stTimeOut);
	if (stat == SOCKET_ERROR) {
		std::cout << std::endl << "wait() function failed" << std::endl;
		stat = 0;
	}
	else if (stat > 0) {
		if (FD_ISSET(s, &stXcptFDS)) {	// Some error was detected or OOB data
			stat = 0;
		}
		if (!FD_ISSET(s, &stReadFDS)) {	// No incoming data!? (check just in case)
			stat = 0;
		}
	}

	// stat will be 0 if stTimeOut exceeded or an error was detected; otherwise stat = 1, 
	// which indicates the socket has pending data.
	return stat;
}