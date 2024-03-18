#include <iostream>
#include <winsock2.h>
#include "Protocol.hpp"
#include "ThreadPool.hpp"
#include "PacketHandler.hpp"
#include <unordered_map>
#include "PlaneData.hpp"
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 27000

int main()
{
	//Init thread pool
	Thread_Pool tp;
	tp.Start(4);
	std::unordered_map<int, planeData> planeList;
	std::unordered_map<int, bool> planeLocks;
	std::mutex planeLocksLock; //lol
	std::list<PlanePacket> messageQueue;
	std::list<PlanePacket>::iterator QuIt;

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

		//if plane is not worked on
		if (planeLocks[received_data.Id] == false) {
			//post the job
			planeLocksLock.lock();
			planeLocks[received_data.Id] = true;
			planeLocksLock.unlock();
			tp.PostJob(PacketHandler::HandleData, received_data, &planeList, &planeLocks, &planeLocksLock);
		}
		else {
			//send to intermediate queue
			messageQueue.push_back(received_data);
		}

		//process intermediate queue
		for (auto iterator = messageQueue.begin(); iterator != messageQueue.end();) {
			//if the plane is not locked
			if (planeLocks[iterator->Id] == false) {
				//dequeue task and post it to the threads
				planeLocksLock.lock();
				planeLocks[iterator->Id] = true;
				planeLocksLock.unlock();
				tp.PostJob(PacketHandler::HandleData, *iterator, &planeList, &planeLocks, &planeLocksLock);
				iterator = messageQueue.erase(iterator);
			}
		}
	}

	// Close socket and cleanup WSA.
	closesocket(server_socket);
	WSACleanup();

	return 0;
}