#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "..\Project6-Server\Protocol.hpp"

#pragma comment(lib, "Ws2_32.lib")

#define PORT 27000
#define SERVER_ADDR L"127.0.0.1"

int main()
{
	// Start Winsock DLLs
	WSADATA wsaData;
	if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		return -1;
	}

	// Initialize client socket. Set SOCK_STREAM to TCP.
	SOCKET client_socket;
	client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client_socket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	// Create socket address for server.
	// Set family to AF_INET for UDP, set port, set IP address.
	sockaddr_in svr_addr;
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(PORT);
	InetPtonW(AF_INET, SERVER_ADDR, &svr_addr.sin_addr.s_addr);



	while (true) {
		// Initialize data.
		PlanePacket data = {
			1,
			time(NULL),
			39.5,
			false
		};

		// Send data to server through client socket.
		sendto(client_socket, (char*)&data, sizeof(PlanePacket), 0, (sockaddr*)&svr_addr, sizeof(svr_addr));

		//Sleep(1);
	}

	// Close socket and clean WSA.
	closesocket(client_socket);
	WSACleanup();

	return 0;
}
