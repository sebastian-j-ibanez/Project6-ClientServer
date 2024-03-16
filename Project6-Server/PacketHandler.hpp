#pragma once
#include "Protocol.hpp"
#include "PlaneData.hpp"
#include <iostream>
#include <unordered_map>
#include <mutex>

class PacketHandler {
public:
	static void HandleData(PlanePacket pkt, std::unordered_map<int, planeData> *planeData, std::unordered_map<int, bool> *dataLocks, std::mutex *dataLocksLock)
	{
		//process data
		pkt.Print();

		//release lock
		dataLocksLock->lock();
		(*dataLocks)[pkt.Id] = false;
		dataLocksLock->unlock();
	}
};