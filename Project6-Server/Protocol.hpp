#pragma once

#include <time.h>
#include <sstream>


/// <summary>
/// Packet containing flight data.
/// </summary>
struct PlanePacket {
    int Id;
    time_t Timestamp;
    float FuelLevel;
    bool EndTransmission;

    void Print() {
        std::stringstream ss;
        ss << "Id:" << Id << " Timestamp: " << Timestamp << " FuelLevel: " << FuelLevel << " EndTransmission: " << EndTransmission;
        std::cout << ss.str() << std::endl;
    }
};

