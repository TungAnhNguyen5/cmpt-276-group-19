// FILE: sailing.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/08/05
//************************************************************
// PURPOSE:
// Implements binary file I/O operations for Sailing records
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/24 - Nathan Miller
//          - Initial .cpp implementation based on header specification.
// Rev. 2 - 2025/08/05 - Nathan Miller
//          - Fully debugged final release version.
//************************************************************

#include "sailing.h"
#include "sailingFileIO.h"
#include "ui.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <limits>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

// cancel is always 0 so using a constant for it makes sense
const int CANCEL_OPTION = 0;

// public functions
bool Sailing::addSailing()
{
    const char SID_SEPARATOR = '-';
    const int CONFIRM_OPTION = 1;

    // uses helper functions to prompt the user for and create the sailing id
    UI::displayHeader("Add Sailing");
    string depTerm = addDepTerm();
    string date = addDate();
    string time = addTime();

    // forms the sailing id according to the ord, converts to char array
    string sidString = depTerm + SID_SEPARATOR + date + SID_SEPARATOR + time;
    char sid[10];

    strcpy(sid, sidString.c_str());
    sid[sizeof(sid) - 1] = '\0';

    // aborts the process if the sailing already exists
    if (sailingFileIO::exists(sid))
    {
        cout << "Sailing with ID " << sid << " already exists.\n";
        return false;
    }

    // uses helper functions to prompt the user for the rest of the sailing information.
    string vessel = addVessel();
    int lcll = addLCLL();
    int hcll = addHCLL();

    // displays the sailing information before prompting the user to confirm
    cout << "\nDeparture Terminal: " << depTerm << "\n";
    cout << "Departure Date: " << date << "\n";
    cout << "Departure Time: " << time << "\n";
    cout << "Vessel ID: " << vessel << "\n";
    cout << "LCLL: " << lcll << "\n";
    cout << "HCLL: " << hcll << "\n";

    // confirms the entry, cancels if cancelled
    if (!confirm(CONFIRM_OPTION))
    {
        // aborts this operation and hands control back to ui.cpp
        return false;
    }

    // combines everything into a sailing
    Sailing s;
    strcpy(s.sailingID, sid);
    strcpy(s.vesselID, vessel.c_str());
    s.lcll = lcll;
    s.hcll = hcll;
    s.lrl = lcll;
    s.hrl = hcll;

    if (sailingFileIO::saveSailing(s))
    {
        cout << "Sailing Successfully Added. Returning to the main menu.\n";
        UI::displayFooter();
        return true;
    }

    cout << "Error: Unable to add sailing.\n";
    // if it fails to save, returns false to the UI
    UI::displayFooter();
    return false;
}

void Sailing::editSailing()
{

    bool editing = true;
    int input;
    Sailing s = *this;

    // runs the editing loop until either confirm, cancel, delete, or manage reservations is chosen
    while (editing)
    {
        // used to keep track of order and options
        enum editSailingOptions
        {
            DEP_TERM = 1,
            DEP_DAY,
            DEP_TIME,
            VESSEL_ID,
            LCLL,
            HCLL,
            NUM_OF_OPTIONS = 6
        };

        string sid = s.sailingID;
        // for loop to print the option select in order
        for (int i = 1; i <= NUM_OF_OPTIONS; i++)
        {
            UI::displayHeader("Edit Sailing");
            switch (i)
            {
            case DEP_TERM:
                cout << "[" << i << "] " << "Departure Terminal: " << sid.substr(0, 3) << "\n";
                break;

            case DEP_DAY:
                cout << "[" << i << "] " << "Departure Day: " << sid.substr(4, 2) << "\n";
                break;

            case DEP_TIME:
                cout << "[" << i << "] " << "Departure Time: " << sid.substr(7, 2) << "\n";
                break;

            case VESSEL_ID:
                cout << "[" << i << "] " << "Vessel ID: " << s.vesselID << "\n";
                break;

            case LCLL:
                cout << "[" << i << "] " << "LCLL: " << s.lcll << "\n";
                break;

            case HCLL:
                cout << "[" << i << "] " << "HCLL: " << s.hcll << "\n";
                break;
            }
        }

        // after two new lines, prints the options that aren't directly tied to editing the sailing.
        const int CONFIRM_OPTION = 7;
        const int RESERVATIONS_OPTION = 8;
        const int DELETE_OPTION = 9;

        cout << "\n[" << CANCEL_OPTION << "] Cancel\n";
        cout << "[" << CONFIRM_OPTION << "] Confirm\n";
        cout << "[" << RESERVATIONS_OPTION << "] Manage Reservations\n";
        cout << "[" << DELETE_OPTION << "] Delete Sailing\n";

        // prompts the user for input until they enter something valid.
        cout << "\nEnter an Option: ";
        cin >> input;
        if (cin.fail() || input < CANCEL_OPTION || input > DELETE_OPTION)
        {
            cout << "Please enter a valid option: ";
        }
        else
        {
            switch (input)
            {
            // returns control to UI if cancelled
            case CANCEL_OPTION:
                cout << "Changes Reverted. Returning to the main menu.\n";
                return;
                break;
            case DEP_TERM:
            {
                string newDepTerm;
                newDepTerm = addDepTerm();
                const char *newDepTermC = newDepTerm.c_str();
                s.sailingID[0] = newDepTermC[0];
                s.sailingID[1] = newDepTermC[1];
                s.sailingID[2] = newDepTermC[2];
                // aborts the operation if the new sailing ID already exists
                if (sailingFileIO::exists(s.sailingID))
                {
                    cout << "Sailing with ID " << s.sailingID << " already exists.\n";
                    return;
                }
                break;
            }
            case DEP_DAY:
            {
                string newDepDay;
                newDepDay = addDate();
                const char *newDepDayC = newDepDay.c_str();
                s.sailingID[4] = newDepDayC[0];
                s.sailingID[5] = newDepDayC[1];
                // aborts the operation if the new sailing ID already exists
                if (sailingFileIO::exists(s.sailingID))
                {
                    cout << "Sailing with ID " << s.sailingID << " already exists.\n";
                    return;
                }
                break;
            }
            case DEP_TIME:
            {
                string newDepTime;
                newDepTime = addTime();
                const char *newDepTimeC = newDepTime.c_str();
                s.sailingID[7] = newDepTimeC[0];
                s.sailingID[8] = newDepTimeC[1];
                // aborts the operation if the new sailing ID already exists
                if (sailingFileIO::exists(s.sailingID))
                {
                    cout << "Sailing with ID " << s.sailingID << " already exists.\n";
                    return;
                }
                break;
            }
            break;
            case VESSEL_ID:
            {
                string newVesselID;
                newVesselID = addVessel();
                const char *newVesselIDC = newVesselID.c_str();
                strncpy(s.vesselID, newVesselIDC, sizeof(s.vesselID) - 1);
                s.vesselID[sizeof(s.vesselID) - 1] = '\0';
                break;
            }
            // ensures that new LCLL and HCLL will still allow for current reservations to fit
            // prints an error and starts the loop over again if user enters an invalid new value
            case LCLL:
            {
                int newLCLL = addLCLL();
                if (newLCLL < s.lrl)
                {
                    cout << "Error: New LCLL is less than lrl\n";
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else
                {
                    s.lrl = newLCLL - (s.lcll - s.lrl);
                    s.lcll = newLCLL;
                }
                break;
            }
            case HCLL:
            {
                int newHCLL = addHCLL();
                if (newHCLL < s.hrl)
                {
                    cout << "Error: New HCLL is less than hrl\n";
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else
                {
                    s.hrl = newHCLL - (s.hcll - s.hrl);
                    s.hcll = newHCLL;
                }
                break;
            }
            case CONFIRM_OPTION:
            {
                editing = false;

                // also needs to call move reservations, add when relevant
                sailingFileIO::deleteSailing(sailingID);

                sailingFileIO::saveSailing(s);

                cout << "Changes Successfully Saved. Returning to the main menu.\n";
                break;
            }
            // opens manage reservations for this sailing, requires reservation class
            case RESERVATIONS_OPTION:
            {
                UI::manageReservationsForSailing(s.sailingID);
                break;
            }
            case DELETE_OPTION:
            {
                editing = false;
                if (sailingFileIO::deleteSailing(s.sailingID))
                {
                    cout << "Sailing Successfully Deleted. Returning to the main menu.\n";
                }
                break;
            }
            }
        }
        UI::displayFooter();
    }
}

void Sailing::displayReport()
{
    // initializes variables required for the for loop
    bool reportActive = true;
    const int VESSEL_ID_LENGTH = 25;
    const int LRL_LENGTH = 6;
    const int HRL_LENGTH = 6;
    const int PERCENT_LENGTH = 4;

    // resets position in the file so it starts at the beginning every time
    sailingFileIO::reset();

    while (reportActive)
    {
        UI::displayHeader("Sailing Report");
        // grabs up to the first five sailings saved in the system
        Sailing *fiveSailings = sailingFileIO::getNextFive();

        cout << "SAILING ID" << "  ";
        cout << left << setw(VESSEL_ID_LENGTH) << "VESSEL ID" << "   ";
        cout << right << setw(LRL_LENGTH) << "LRL" << "   ";
        cout << right << setw(HRL_LENGTH) << "HRL" << "  ";
        cout << right << "FULL %" << "\n";
        // iterates through the sailing(s) grabbed
        for (int i = 0; i < 5; i++)
        {
            if (fiveSailings[i].getSailingID()[0] == '\0')
            {
                // if the sailingID is empty, it means there are no more sailings to display, user can loop through them again with '5'
                sailingFileIO::reset();
                break;
            }
            // prints the sailing information formatted according to the user manual
            cout << fiveSailings[i].sailingID << "   ";
            cout << left << setw(VESSEL_ID_LENGTH) << fiveSailings[i].vesselID << "   ";
            cout << right << setw(LRL_LENGTH) << fiveSailings[i].lrl << "   ";
            cout << right << setw(HRL_LENGTH) << fiveSailings[i].hrl << "   ";
            int percentFull = (1 - ((fiveSailings[i].lrl + fiveSailings[i].hrl) / (fiveSailings[i].lcll + fiveSailings[i].hcll))) * 100;
            cout << right << setw(PERCENT_LENGTH) << percentFull << "%\n";
        }

        // prompts the user to enter an option, continues until valid input is received
        cout << "\n[0] Cancel\n[5] Show next 5\n\nEnter an option: ";
        int input;
        bool validInput = false;
        while (!validInput)
        {
            cin >> input;
            if (cin.fail() || (input != CANCEL_OPTION && input != 5))
            {
                cout << "Invalid option, please enter an option: ";
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else
            {
                validInput = true;
                if (input == CANCEL_OPTION)
                {
                    reportActive = false;
                }
            }
        }
        UI::displayFooter();
    }
}

// nothing needs to be initialized or shutdown here so these are just to initialize and shutdown file IO.
void Sailing::initialize()
{
    sailingFileIO::openFile();
}
void Sailing::shutdown()
{
    sailingFileIO::closeFile();
}

const char *Sailing::getSailingID() const
{
    return sailingID;
}

Sailing Sailing::getSailingFromIO(const char *sid)
{
    return sailingFileIO::getSailing(sid);
}

// functions for updating the lrl and hrl of a specific sailing, subtracts f from lrl or hrl
bool Sailing::lrlUpdate(float f)
{
    if (lrl - f >= 0)
    {
        lrl -= f;
        sailingFileIO::saveSailing(*this);
        return true;
    }
    return false;
}

float Sailing::lrlRemaining() const
{
    return lrl;
}

bool Sailing::hrlUpdate(float f)
{
    if (hrl - f >= 0)
    {
        hrl -= f;
        sailingFileIO::saveSailing(*this);
        return true;
    }
    return false;
}

float Sailing::hrlRemaining() const
{
    return hrl;
}

// Add these getter function implementations after the existing functions:

const char *Sailing::getVesselID() const {
    return vesselID;
}

int Sailing::getLCLL() const {
    return lcll;
}

int Sailing::getHCLL() const {
    return hcll;
}

float Sailing::getLRL() const {
    return lrl;
}

float Sailing::getHRL() const {
    return hrl;
}

// private functions
string Sailing::addDepTerm()
{
    bool validEntry = false;
    const int DEP_TERM_LENGTH = 3;
    string depTerm;
    while (!validEntry)
    {
        cout << "Enter a departure terminal: ";

        cin >> depTerm;

        // departure terminal is represented by 3 characters
        if (depTerm.length() != DEP_TERM_LENGTH)
        {
            cout << "Invalid departure terminal.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            validEntry = true;
        }
    }
    return depTerm;
}

string Sailing::addDate()
{
    bool validEntry = false;
    const int DATE_LENGTH = 2;
    string date;
    while (!validEntry)
    {
        cout << "Enter a departure date: ";

        cin >> date;
        // date entered should be two digits to represent the date, month is not relevant
        if (date.length() != DATE_LENGTH || !std::all_of(date.begin(), date.end(), ::isdigit) || stoi(date) < 0 || stoi(date) > 28)
        {
            cout << "Invalid departure date.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            validEntry = true;
        }
    }
    return date;
}

string Sailing::addTime()
{
    bool validEntry = false;
    const int TIME_LENGTH = 2;
    string time;
    while (!validEntry)
    {
        cout << "Enter a departure time: ";

        cin >> time;
        // time entered should be two digits in 24 hour time, can't be below 0 or above 23
        if (time.length() != TIME_LENGTH || !std::all_of(time.begin(), time.end(), ::isdigit) || stoi(time) < 0 || stoi(time) > 23)
        {
            cout << "Invalid departure time.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            validEntry = true;
        }
    }
    return time;
}

string Sailing::addVessel()
{
    bool validEntry = false;
    const int VESSEL_LENGTH = 25;
    string vessel;
    while (!validEntry)
    {
        cout << "Enter a vessel ID: ";

        cin >> vessel;
        // vesselID can be between 1 and 25 characters
        if (vessel.length() > VESSEL_LENGTH || vessel.length() == 0)
        {
            cout << "Invalid vessel ID.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            validEntry = true;
        }
    }
    return vessel;
}

int Sailing::addLCLL()
{
    bool validEntry = false;
    int lcll;
    const int LCLL_MAX = 9999;

    while (!validEntry)
    {
        cout << "Enter LCLL: ";
        cin >> lcll;

        // reprompts user if entered value is not a float or too big/small.
        // value is converted to DDDD.D form when saved.
        if (cin.fail() || lcll > LCLL_MAX || lcll < 0)
        {
            cout << "Invalid LCLL.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            validEntry = true;
        }
    }
    return lcll;
}

int Sailing::addHCLL()
{
    bool validEntry = false;
    int hcll;
    const int HCLL_MAX = 9999;

    while (!validEntry)
    {
        cout << "Enter HCLL: ";
        cin >> hcll;

        // reprompts user if entered value is not a float or too big/small.
        // value is converted to DDDD.D form when saved.
        if (cin.fail() || hcll > HCLL_MAX || hcll < 0)
        {
            cout << "Invalid HCLL.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            validEntry = true;
        }
    }
    return hcll;
}

bool Sailing::confirm(int confirmOption)
{
    while (true)
    {
        int input;
        cout << "\n[" << CANCEL_OPTION << "] Cancel\n";
        cout << "[" << confirmOption << "] Confirm\n\n";
        cout << "Enter an option: ";
        cin >> input;
        if (cin.fail() || (input != CANCEL_OPTION && input != confirmOption))
        {
            cout << "Invalid option.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if (input == CANCEL_OPTION)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    return true;
}

void Sailing::createSailing(string line) {
    // Initialize with empty values first
    memset(sailingID, 0, sizeof(sailingID));
    memset(vesselID, 0, sizeof(vesselID));
    lcll = 0;
    hcll = 0;
    lrl = 0.0f;
    hrl = 0.0f;
    
    if (line.empty()) {
        return;
    }
    
    try {
        // Parse the line using delimiter '|'
        vector<string> fields;
        stringstream ss(line);
        string field;
        
        // Split by delimiter
        while (getline(ss, field, '|')) {
            fields.push_back(field);
        }
        
        // Need at least 6 fields: sailingID|vesselID|lcll|hcll|lrl|hrl
        if (fields.size() >= 6) {
            // Copy sailing ID (max 9 chars + null terminator)
            strncpy(sailingID, fields[0].c_str(), sizeof(sailingID) - 1);
            sailingID[sizeof(sailingID) - 1] = '\0';
            
            // Copy vessel ID (max 25 chars + null terminator)
            strncpy(vesselID, fields[1].c_str(), sizeof(vesselID) - 1);
            vesselID[sizeof(vesselID) - 1] = '\0';
            
            // Parse numeric fields
            lcll = stoi(fields[2]);
            hcll = stoi(fields[3]);
            lrl = stof(fields[4]);
            hrl = stof(fields[5]);
        }
    } catch (const exception& e) {
        cerr << "Error parsing sailing line: " << e.what() << endl;
        // Reset to empty state on error
        memset(sailingID, 0, sizeof(sailingID));
        memset(vesselID, 0, sizeof(vesselID));
        lcll = 0;
        hcll = 0;
        lrl = 0.0f;
        hrl = 0.0f;
    }
}
