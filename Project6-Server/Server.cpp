#include <iostream>
#include <winsock2.h>
#include "Protocol.hpp"
#include "ThreadPool.hpp"
#include "PacketHandler.hpp"
#include "PlaneData.hpp"
#include "FileIO.hpp"

#pragma comment(lib, "Ws2_32.lib")

#define PORT 27000

int main()
{
	//Init thread pool
	Thread_Pool tp;
	tp.Start(4);

	// Start Winsock DLLs		
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		return -1;

	// Create server socket
	SOCKET server_socket;
	server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (server_socket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	// Bind socket to server address
	sockaddr_in svr_addr;
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	svr_addr.sin_port = htons(PORT);
	
	// Bind socket to address and check for errors.
	if (bind(server_socket, (struct sockaddr*)&svr_addr, sizeof(svr_addr)) == SOCKET_ERROR)
	{
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}

	// Receive message from Connection Socket.
	while (true) {
		PlanePacket received_data = {};
		sockaddr_in clt_addr;
		int clt_addr_size = sizeof(clt_addr);
		int bytes = recvfrom(server_socket, (char*)&received_data, sizeof(PlanePacket), 0, (sockaddr*)&clt_addr, &clt_addr_size);
		
		if (bytes == SOCKET_ERROR) {
			closesocket(server_socket);
			WSACleanup();
			return -1;
		}

		//handling here
		tp.PostJob(PacketHandler::HandleData, received_data);

		//std::cout << "Received: " << std:: endl;
		//std::cout << received_data.Id << " " << received_data.Timestamp << " " << received_data.FuelLevel << " " << received_data.EndTransmission << std::endl;
	}

	// Close socket and cleanup WSA.
	closesocket(server_socket);
	WSACleanup();

	return 0;
}