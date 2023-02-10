#pragma once
// TicTacToe.h
//
// This header file establishes some constants that should be used
// in the TicTacToe project for COMP 3110
#include <WinSock2.h>
#include <string>
#include <vector>

static char NIM_UDPPORT[] = "29333"; 	// Port number used by TicTacToe servers

#define NIM_QUERY  "Who?"
#define TicTacToe_NAME	 "Name="
#define TicTacToe_CHALLENGE "Player="
#define YES "YES"
#define NO "NO"


const int MAX_SEND_BUF = 2048;
const int MAX_RECV_BUF = 2048;
const int MAX_SERVERS = 100;
const int X_PLAYER = 1;
const int O_PLAYER = 2;
const int ABORT = -1;
const int noWinner = 0;
const int xWinner = 1;
const int oWinner = 2;
const int TIE = 3;

static char board[10];

struct ServerStruct {
	std::string name;
	std::string host;
	std::string port;
};

SOCKET connectsock(char*, char*, char*);
SOCKET passivesock(char*, char*);
int UDP_recv(SOCKET, char*, int, char*, char*);
int UDP_send(SOCKET, char*, int, char*, char*);
int wait(SOCKET, int, int);
char* GetBroadcastAddress();

int server_main(int, char* [], std::string);
int client_main(int, char* [], std::string);
void getServers(SOCKET, char*, char*, ServerStruct[], int&);
int playNim(SOCKET, std::string, std::string, std::string, int, char*);
//void displayBoard(vector<int>);


