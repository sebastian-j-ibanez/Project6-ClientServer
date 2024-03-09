#pragma once

#include <time.h>

/// <summary>
/// Packet containing flight data.
/// </summary>
struct PlanePacket {
    int Id;
    time_t Timestamp;
    float FuelLevel;
    bool EndTransmission;
};
