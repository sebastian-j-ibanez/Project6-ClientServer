#pragma once

#include <iostream>
#include <sstream>

struct planeData {
	float fuelCompAvg;
	int numTrans;

    // Return formatted string with PlaneData fields.
    std::string ToString() {
        std::stringstream ss;
        ss << "Fuel Consumption Average:" << fuelCompAvg << std::endl << "Number of transmissions:" << numTrans << std::endl;
        return ss.str();
    }

    // Print PlaneData to std::cout.
    void Print() {
        std::cout << ToString() << std::endl;
    }
};