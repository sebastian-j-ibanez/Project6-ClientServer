#pragma once
#include "Protocol.hpp"
#include "PlaneData.hpp"
#include "FileIO.hpp"
#include <iostream>
#include <unordered_map>
#include <mutex>
#include <map>
#include <ctime>

class PacketHandler {
public:
	static void HandleData(PlanePacket pkt, planeData* currentPlaneData)
	{
		currentPlaneData->numTrans++;
		// If first transmission of this plane's data, set initialFuel and initialTime values for future calculations
		if (currentPlaneData->numTrans == 1)
		{
			currentPlaneData->initialFuel = pkt.FuelLevel;
			currentPlaneData->initialTime = pkt.Timestamp;
		}
		// Create a fuelCompAvg if this is the second trasmission or the tenth transmission since the last Avg calculation. Also calculate avg on last transmission
		else if (!pkt.EndTransmission)
		{
			float fuelDif = ((pkt.FuelLevel - currentPlaneData->initialFuel) * -1);		// Multiply by -1 because the difference causes a negative but should be reflected by a positive in terms of consumption of fuel per second
			double timeDif = difftime(pkt.Timestamp, currentPlaneData->initialTime);	// difftime should return the amount of seconds between the end time and the start time
			currentPlaneData->fuelCompAvg = fuelDif / timeDif;
		}

		// Write average to file if last packet of data transmission
		if (pkt.EndTransmission)
			WriteToFile(pkt.Id, *currentPlaneData);
	}
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