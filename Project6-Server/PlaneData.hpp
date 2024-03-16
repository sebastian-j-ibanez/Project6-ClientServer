#pragma once

#include <iostream>
#include <sstream>

struct planeData {
	float fuelCompAvg;
    time_t initialTime;
	float initialFuel;
	int numTrans;

    // Default constructor sets all values to 0
    planeData()     
    {
        fuelCompAvg = 0;
        initialTime = time(NULL);
        initialFuel = 0;
        numTrans = 0;
    }

    // Return formatted string with PlaneData fields.
    std::string ToString() {
        std::stringstream ss;
        ss << "Fuel Consumption Average: " << fuelCompAvg * 1000 << "mL/s" << std::endl << "Number of transmissions: " << numTrans << std::endl;
        return ss.str();
    }

    // Print PlaneData to std::cout.
    void Print() {
        std::cout << ToString() << std::endl;
    }
};