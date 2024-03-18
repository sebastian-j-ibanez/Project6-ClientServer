#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include "..\Project6-Server\Protocol.hpp"

#pragma comment(lib, "Ws2_32.lib")

#define PORT 27000
#define SERVER_ADDR L"127.0.0.1"
#define DATE "2024-03-17 "


using namespace std;

// Converts string formatted as H:M:S into time_t object.
time_t TimeConvert(string TimeString)
{
    // Format TimeString and initialize tm using get_time.
    TimeString = DATE + TimeString;
    std::istringstream ss(TimeString);
    struct std::tm tm = { 0 };
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    // Check for parsing errors
    if (ss.fail()) {
        std::cerr << "Error parsing time string" << std::endl;
    }
    // Convert tm to time_t and return value.
    std::time_t time = std::mktime(&tm);
    return time;
}

int main()
{
	// Start Winsock DLLs
	WSADATA wsaData;
	if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		return -1;
	}

	// Initialize client socket. Set SOCK_STREAM to TCP.
	SOCKET client_socket;
	client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client_socket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	// Create socket address for server.
	// Set family to AF_INET for UDP, set port, set IP address.
	sockaddr_in svr_addr;
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(PORT);
	InetPtonW(AF_INET, SERVER_ADDR, &svr_addr.sin_addr.s_addr);

	// Initialize data.

    ifstream TelemFile;
    TelemFile.open("katl-kefd-B737-700.txt");
    if (!TelemFile.is_open())
    {
        cout << "File Failed to Open" << endl;
        return 1;
    }
    const int id = 1; // temporary ID for PlanePacket
    float CurrentFuel; // The current Fuel passed in a specific Line
    string CurrentFuelString; // we cant use ss on a double so we use this as a placeholder
    time_t Time;
    string CurrentTime; // The current time passed in a specific line, need to convert to time_t

    string Line; // Current Line we are reading
    string Discarded; // Discared part of the string that we are not using, gets discarded

    // Remove first line (only contains column names).
    std::getline(TelemFile, Line);

    // Read file by line. Parse each line for CurrentTime and CurrentFuel.
    while (std::getline(TelemFile, Line)) {

        istringstream ss(Line);

        // Get rid of time stamp at beginning of line.
        std::getline(ss, Discarded, ' ');

        // Get CurrentTime (HH:MM:SS format) and convert to time_t.
        std::getline(ss, CurrentTime, ',');
        Time = TimeConvert(CurrentTime);

        std::getline(ss, CurrentFuelString, ','); // Next section of the line will be the Fuel
        CurrentFuel = stof(CurrentFuelString); // Convert the String Current Fuel to a float

        PlanePacket data = {
            id,
            Time,
            CurrentFuel,
            false
        };

        // Send data to server through client socket.
        sendto(client_socket, (char*)&data, sizeof(PlanePacket), 0, (sockaddr*)&svr_addr, sizeof(svr_addr));
    }

    // Send packet with endTransmission flag set.
    PlanePacket endPacket = {
        id, 
        time(NULL),
        0,
        true
    };

    // Send endPacket to server through client socket.
    sendto(client_socket, (char*)&endPacket, sizeof(PlanePacket), 0, (sockaddr*)&svr_addr, sizeof(svr_addr));

	}

		//Sleep(1);
	}

	// Close socket and clean WSA.
	closesocket(client_socket);
	WSACleanup();

	return 0;
}
