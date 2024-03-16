#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "..\Project6-Server\Protocol.hpp"
#include <fstream>
#include <string>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 27000
#define SERVER_ADDR L"127.0.0.1"
using namespace std;
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
	PlanePacket data = {
		1,
		time(NULL),
		39.5,
		false
	};

	// Send data to server through client socket.
	sendto(client_socket, (char*)&data, sizeof(PlanePacket), 0, (sockaddr*)&svr_addr, sizeof(svr_addr));

	// Close socket and clean WSA.
	closesocket(client_socket);
	WSACleanup();

	return 0;
}

int parse()
{

    ifstream TelemFile;
    TelemFile.open("katl-kefd-B737-700.txt");


    if (!TelemFile.is_open())
    {
        cout << "File Failed to Open" << endl;
        return 0;
    }

    double CurrentFuelAverage = 0; // The current average of the fuel Consumptation
    double CurrentFuel; // The current Fuel passed in a specific Line
    string CurrentFuelString; // we cant use ss on a double so we use this as a placeholder
    double TotalFuel = 0; // The total fuel that gets summed by every fuel value


    string CurrentTime; // The current time passed in a specific line, need to convert to time_t
    string LatestTimeInterval; // Latest time for the 10 second time interval, Not Used currently

    string Line; // Current Line we are reading
    string Discarded; // Discared part of the string that we are not using, gets discarded
    int LineNumber = 1;

    getline(TelemFile, Line);
    do
    {

        istringstream ss(Line);

        if (LineNumber == 1) // skip FUEL TOTAL QUANTITY in the first line
        {
            getline(ss, Discarded, ',');
        }
        else
        {
            getline(ss, Discarded, ' ');

        }
        getline(ss, CurrentTime, ',');
        CurrentTime = CurrentTime.substr(9, 8); // we do not need the date, only the time

        getline(ss, CurrentFuelString, ','); // next section of the line will be the Fuel
        CurrentFuel = stod(CurrentFuelString); // convert the String Current Fuel to a Double

        TotalFuel += CurrentFuel;

        CurrentFuelAverage = (TotalFuel / LineNumber); // Calculating the Average

        cout << CurrentFuel << endl; // currently prints CurrentFuel that gets passed in a line, can change to other values

        LineNumber++; // we can use the line number to keep track of the total number of current fuels for the average
        getline(TelemFile, Line);

    } while (!TelemFile.eof());
}
