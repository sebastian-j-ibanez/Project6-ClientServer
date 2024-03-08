// THIS FILE HAS BEEN INITIALIZED USING A SIMPLE DEMO FROM WEEK 2 OF CSCN72050.
// THIS CODE HAS BEEN AUTHORED BY DR. COLESHILL, AND IS A PLACEHOLDER FOR OUR SERVER.

#include <windows.networking.sockets.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void main()
{
	// Start Winsock DLLs		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return;

	// Create server socket
	SOCKET ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET) {
		WSACleanup();
		return;
	}

	// Bind socket to server address
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(27000);
	if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
	{
		closesocket(ServerSocket);
		WSACleanup();
		return;
	}

	// Listen on a socket
	if (listen(ServerSocket, 1) == SOCKET_ERROR) {
		closesocket(ServerSocket);
		WSACleanup();
		return;
	}


	cout << "Waiting for client connection\n" << endl;

	// Accept a connection from a client
	SOCKET ConnectionSocket;
	ConnectionSocket = SOCKET_ERROR;
	if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) {
		closesocket(ServerSocket);
		WSACleanup();
		return;
	}

	cout << "Connection Established" << endl;

	// Receive message from Connection Socket.
	char RxBuffer[128] = {};
	recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
	cout << "Msg Rx: " << RxBuffer << endl;

	closesocket(ConnectionSocket);	// Close incoming socket
	closesocket(ServerSocket);	    // Close server socket	
	WSACleanup();					// Free Winsock resources
}