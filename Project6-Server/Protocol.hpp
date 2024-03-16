#pragma once

#include <time.h>
#include <sstream>

// Packet containing flight data.
struct PlanePacket {
    int Id;
    time_t Timestamp;
    float FuelLevel;
    bool EndTransmission;

    //  Return formatted string with PlanePacket fields.
    std::string ToString() {
        std::stringstream ss;
        ss << "Id:" << Id << " Timestamp: " << Timestamp << " FuelLevel: " << FuelLevel << " EndTransmission: " << EndTransmission;
        return ss.str();
    }

    // Print PlanePacket to std::cout.
    void Print() {
        std::cout << ToString() << std::endl;
    }
};