// server_main.cpp
//   This function serves as the "main" function for the server-side
#include "Nim.h"
#include <iostream>
#include <string>
#include <ChString.h>
#include <vector>
#include <iomanip>
#include <WinSock2.h>
using namespace std;

int concat(int a, int b)
{
	string s1 = to_string(a);
	string s2 = to_string(b);
	string s = s1 + s2;
	int c = stoi(s);
	return c;
}

string checkPiles(int columns) {

	string piles, totalPiles;
	string pairs;

	for (int i = 0; i < columns; i++) {
		cout << "Input the number of rocks for pile " << i + 1 << ": ";
		cin >> piles;
		if (piles.length() != 2) {
			cout << "Input again: ";
			cin >> piles;
		}

		for (int i = 0; i < 1; i++) {
			int fdigit = piles[i] - '0';
			int sdigit = piles[i + 1] - '0';
			if (fdigit == 0) {
				if (sdigit > 1 && sdigit <= 20) {
					pairs += to_string(0);
					pairs += to_string(sdigit);
				}
			}
			else {
				int pair = concat(piles[i] - '0', piles[i + 1] - '0');
				if (pair > 1 && pair <= 20) {
					pairs += to_string(pair);
				}
			}
		}
	}
	return pairs;
}

string getPiles() {
	std::cout << std::endl;
	bool check = true;
	string boardInput;
	int columns;
	cout << "Input number of piles for the Nim Game: ";
	cin >> columns;

	while (check) {
		if (columns >= 3 && columns <= 9) {
			boardInput = checkPiles(columns);
			check = false;
		}
		else {
			std::cout << "Input piles again: ";
			std::cin >> columns;
			std::cout << std::endl;
			check = true;
		}
	}
	return boardInput;
}

int server_main(int argc, char* argv[], std::string playerName)
{
	SOCKET s;
	char buf[MAX_RECV_BUF];
	std::string host;
	std::string port;
	char response_str[MAX_SEND_BUF];
	char protocol[4] = "udp";

	s = passivesock(NIM_UDPPORT, protocol);

	std::cout << std::endl << "Waiting for a challenge..." << std::endl;
	int len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
	std::cout << "Received: " << buf << std::endl;  // For DEBUG purposes

	bool finished = false;
	while (!finished) {
		if (strcmp(buf, NIM_QUERY) == 0) {
			// Respond to name query
			strcpy_s(response_str, TicTacToe_NAME);
			strcat_s(response_str, playerName.c_str());
			UDP_send(s, response_str, strlen(response_str) + 1, (char*)host.c_str(), (char*)port.c_str());
			std::cout << "Sending: " << response_str << std::endl;	// DEBUG

		}
		else if (strncmp(buf, TicTacToe_CHALLENGE, strlen(TicTacToe_CHALLENGE)) == 0) {
			// Received a challenge  
			char* startOfName = strstr(buf, TicTacToe_CHALLENGE);
			if (startOfName != NULL) {
				std::cout << std::endl << "You have been challenged by " << startOfName + strlen(TicTacToe_CHALLENGE) << std::endl;
			}

			// Play the game.  You are the 'O' player

			//get input
			string input;
			input = getPiles();

			UDP_send(s, (char*)input.c_str(), strlen((char*)input.c_str()) + 1, (char*)host.c_str(), (char*)port.c_str());
			int winner = playNim(s, (char*)playerName.c_str(), (char*)host.c_str(), (char*)port.c_str(), O_PLAYER, (char*)input.c_str());
			finished = true;
		}

		if (!finished) {
			char previous_buf[MAX_RECV_BUF];		strcpy_s(previous_buf, buf);
			std::string previous_host;				previous_host = host;
			std::string previous_port;				previous_port = port;

			// Check for duplicate datagrams (can happen if broadcast enters from multiple ethernet connections)
			bool newDatagram = false;
			int status = wait(s, 1, 0);	// We'll wait a second to see if we receive another datagram
			while (!newDatagram && status > 0) {
				len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				std::cout << "Received: " << buf << std::endl;	// DEBUG
				if (strcmp(buf, previous_buf) == 0 &&		// If this datagram is identical to previous one, ignore it.
					host == previous_host &&
					port == previous_port) {
					status = wait(s, 1, 0);			// Wait another second (still more copies?)
				}
				else {
					newDatagram = true;		// if not identical to previous one, keep it!
				}
			}
			// If we waited one (or more seconds) and received no new datagrams, wait for one now.
			if (!newDatagram) {
				len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				std::cout << "Received: " << buf << std::endl;	// DEBUG
			}
		}
	}
	closesocket(s);

	return 0;
}