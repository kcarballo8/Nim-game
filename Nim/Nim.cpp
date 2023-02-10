// TicTacToe.cpp

// This program plays Tic-Tac-Toe with other computers across the network using the UDP protocol.

#include "Nim.h"
#include <iostream>
#include <string>
#include <WinSock2.h>
using namespace std;

#pragma comment (lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	WORD wVersion = 0x0202;
	WSADATA wsaData;
	int iResult = WSAStartup(wVersion, &wsaData);
	if (iResult != 0) {
		std::cout << "Unable to initialize Windows Socket library." << std::endl;
		return 0;
	}
	char playerName[80];
	std::cout << "What is your name? ";
	cin.getline(playerName, 80);


	std::string choice_str;
	char newline;
	int choice = 0;
	const int QUIT = 3;
	do {
		std::cout << std::endl << std::endl << std::endl;
		std::cout << "Choose an option:" << std::endl;
		std::cout << "   1 - Host a game of Nim" << std::endl;
		std::cout << "   2 - Challenge someone else" << std::endl;
		std::cout << "   3 - Quit" << std::endl;
		std::cout << "Enter 1, 2 or 3: ";
		std::cin >> choice_str; std::cin.get(newline);
		choice = atoi(choice_str.c_str());

		switch (choice) {
		case 1:	server_main(argc, argv, playerName); break;
		case 2:	client_main(argc, argv, playerName); break;
		case 3: break;
		default: std::cout << std::endl << "Please enter a digit between 1 and 3." << std::endl;
		}
	} while (choice != QUIT);

	WSACleanup();
}

