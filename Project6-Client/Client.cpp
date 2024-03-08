// THIS FILE HAS BEEN INITIALIZED USING A SIMPLE DEMO FROM WEEK 2 OF CSCN72050.
// THIS CODE HAS BEEN AUTHORED BY DR. COLESHILL, AND IS A PLACEHOLDER FOR OUR CLIENT.

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
using namespace std;

void main()
{
	// Start Winsock DLLs
	WSADATA wsaData;
	if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		return;
	}

	// Initialize client socket. Set SOCK_STREAM to TCP.
	SOCKET ClientSocket;
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		WSACleanup();
		return;
	}

	// Connect socket to specified server
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;						// Address family type internet
	SvrAddr.sin_port = htons(27000);					// Port (host to network conversion)
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	// IP address
	if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
		closesocket(ClientSocket);
		WSACleanup();
		return;
	}

	char TxBuffer[128] = {};
	cout << "Enter a String to transmit" << endl;
	cin >> TxBuffer;
	send(ClientSocket, TxBuffer, sizeof(TxBuffer), 0);

	// Close connection and socket
	closesocket(ClientSocket);

	// Free Winsock DLL resources
	WSACleanup();
}
