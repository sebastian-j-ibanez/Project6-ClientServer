#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main()
{
	// Start Winsock DLLs		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	// Create server socket
	SOCKET ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
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
		return -1;
	}

	// Listen on a socket
	if (listen(ServerSocket, 1) == SOCKET_ERROR) {
		closesocket(ServerSocket);
		WSACleanup();
		return -1;
	}


	cout << "Waiting for client connection\n" << endl;

	// Accept a connection from a client
	SOCKET ConnectionSocket;
	ConnectionSocket = SOCKET_ERROR;
	if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) {
		closesocket(ServerSocket);
		WSACleanup();
		return -1;
	}

	cout << "Connection Established" << endl;

	// Receive message from Connection Socket.
	char RxBuffer[128] = {};
	recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
	cout << "Msg Rx: " << RxBuffer << endl;

	closesocket(ConnectionSocket);
	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}