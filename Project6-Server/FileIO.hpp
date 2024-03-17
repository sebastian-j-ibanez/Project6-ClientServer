#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "PlaneData.hpp"

#define FILEPATH "Log/"

// Write PlaneData to file.
void WriteToFile(int id, planeData data) {
    std::string string_id = std::to_string(id);

    // Construct file name using FILEPATH and Id.
    std::string file_name = FILEPATH + string_id + ".txt";

    // Open file stream in trunc mode, to overwrite file if contents exist.
    std::ofstream file;
    file.open(file_name, std::ios::trunc);

    // Check file stream status, write data to file, and close file.
    if (file.is_open()) {
        file << "Id: " << string_id << '\n' << data.ToString() << '\n';
        file.close();
    }
    else {
        std::cout << "Error writing to " << file_name << std::endl;
    }
}