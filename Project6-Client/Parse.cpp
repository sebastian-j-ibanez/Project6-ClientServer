#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <string>

using namespace std;

int main()
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
    double TotalFuel = 0;


    string CurrentTime; // The current time passed in a specific line, need to convert to time_t
    string LatestTimeInterval; // Latest time for the 10 second time interval

    string Line; // Current Line we are reading
    string Discarded;
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

        cout << CurrentFuel << endl;

        LineNumber++; // we can use the line number to keep track of the total number of current fuels for the average
        getline(TelemFile, Line);

    } while (!TelemFile.eof());

}