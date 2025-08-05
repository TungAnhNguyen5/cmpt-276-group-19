// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
//   Implements the Sailing class, which manages sailing records
//   including creation, editing, and persistence through file //I/O.
//   Provides functionality for adding, editing, and retrieving //sailings,
//   as well as handling user input for sailing management.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - Nathan Miller
//          - Initial interface for sailing class.
// Rev. 2 - 2025/07/24 - Nathan Miller
//          - Finalized format and synced with .cpp logic.
// Rev. 3 - 2025/08/05 - Nathan Miller
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
    
    // Validate that this sailing has valid data before proceeding
    if (strlen(s.sailingID) == 0 || s.sailingID[0] == '\0') {
        UI::displayHeader("Edit Sailing");
        cout << "Error: Invalid sailing data. Cannot edit this sailing.\n";
        UI::displayFooter();
        return;
    }

    // runs the editing loop until either confirm, cancel, delete, or manage reservations is chosen
    // Loop goal: Continue editing sailing details until user chooses to confirm, cancel, delete, or manage reservations
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
        
        // Additional validation: check if sailing ID has proper format
        if (sid.length() < 7 || sid.find('-') == string::npos) {
            UI::displayHeader("Edit Sailing");
            cout << "Error: Invalid sailing ID format. Cannot edit this sailing.\n";
            UI::displayFooter();
            return;
        }
        
        UI::displayHeader("Edit Sailing");
        // Loop goal: Display all editing options in order (1 through NUM_OF_OPTIONS)
        for (int i = 1; i <= NUM_OF_OPTIONS; i++)
        {            
            switch (i)
            {
            case DEP_TERM:
                if (sid.length() >= 3) {
                    cout << "[" << i << "] " << "Departure Terminal: " << sid.substr(0, 3) << "\n";
                } else {
                    cout << "[" << i << "] " << "Departure Terminal: [Invalid]\n";
                }
                break;

            case DEP_DAY:
                if (sid.length() >= 6) {
                    cout << "[" << i << "] " << "Departure Day: " << sid.substr(4, 2) << "\n";
                } else {
                    cout << "[" << i << "] " << "Departure Day: [Invalid]\n";
                }
                break;

            case DEP_TIME:
                if (sid.length() >= 9) {
                    cout << "[" << i << "] " << "Departure Time: " << sid.substr(7, 2) << "\n";
                } else {
                    cout << "[" << i << "] " << "Departure Time: [Invalid]\n";
                }
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
            cout << "Please enter a valid option.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            UI::displayFooter();
            continue; // Go back to the beginning of the loop
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
    const int PERCENT_LENGTH = 6;

    // resets position in the file so it starts at the beginning every time
    sailingFileIO::reset();

    // Loop goal: Continue displaying sailing reports until user chooses to exit
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
        // Loop goal: Display up to 5 sailing records from the current file position
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
            
            // Calculate and display capacity percentage
            float percentFull = calculateCapacityPercentage(fiveSailings[i].lcll, fiveSailings[i].hcll, 
                                                         fiveSailings[i].lrl, fiveSailings[i].hrl);
            cout << right << setw(PERCENT_LENGTH) << fixed << setprecision(1) << percentFull << "%";
            cout << resetiosflags(ios::fixed) << setprecision(6) << "\n"; // Reset formatting
        }

        // prompts the user to enter an option, continues until valid input is received
        cout << "\n[0] Cancel\n[5] Show next 5\n\nEnter an option: ";
        int input;
        bool validInput = false;
        // Loop goal: Keep prompting until user enters a valid option (0 or 5)
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
    // Loop goal: Keep prompting until user enters a valid 3-character departure terminal
    while (!validEntry)
    {
        cout << "Format: 3-letter terminal code (e.g., TSA, SWB, HGB)\n";
        cout << "Enter a departure terminal: ";

        cin >> depTerm;

        // departure terminal is represented by 3 characters
        if (depTerm.length() != DEP_TERM_LENGTH)
        {
            cout << "Invalid departure terminal. Must be exactly 3 characters.\n";
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
    // Loop goal: Keep prompting until user enters a valid 2-digit date (01-28)
    while (!validEntry)
    {
        cout << "Format: 2-digit day (01-28, e.g., 01, 15, 28)\n";
        cout << "Enter a departure date: ";

        cin >> date;
        // date entered should be two digits to represent the date, month is not relevant
        if (date.length() != DATE_LENGTH || !std::all_of(date.begin(), date.end(), ::isdigit) || stoi(date) < 0 || stoi(date) > 28)
        {
            cout << "Invalid departure date. Must be 2 digits between 01-28.\n";
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
    // Loop goal: Keep prompting until user enters a valid 2-digit time (00-23)
    while (!validEntry)
    {
        cout << "Format: 2-digit hour in 24-hour format (00-23, e.g., 08, 14, 23)\n";
        cout << "Enter a departure time: ";

        cin >> time;
        // time entered should be two digits in 24 hour time, can't be below 0 or above 23
        if (time.length() != TIME_LENGTH || !std::all_of(time.begin(), time.end(), ::isdigit) || stoi(time) < 0 || stoi(time) > 23)
        {
            cout << "Invalid departure time. Must be 2 digits between 00-23.\n";
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
    // Loop goal: Keep prompting until user enters a valid vessel ID (1-25 characters)
    while (!validEntry)
    {
        cout << "\nVessel ID/Name:\n";
        cout << "Format: Vessel name or identifier\n";
        cout << "Guidelines:\n";
        cout << "  - 1-25 characters maximum\n";
        cout << "  - Can be name or alphanumeric ID\n";
        cout << "  - No spaces (use underscore if needed)\n";
        cout << "  - Examples: CoastalRunner, Island_Voyager, FERRY123, Vessel_01\n";
        cout << "Enter a vessel ID: ";

        cin >> vessel;
        // vesselID can be between 1 and 25 characters
        if (vessel.length() > VESSEL_LENGTH || vessel.length() == 0)
        {
            cout << "Invalid vessel ID. Must be 1-25 characters long.\n";
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

    // Loop goal: Keep prompting until user enters a valid LCLL value (0-9999)
    while (!validEntry)
    {
        cout << "\nLow Car Length Limit (LCLL):\n";
        cout << "Format: Maximum length for regular vehicles in meters\n";
        cout << "Guidelines:\n";
        cout << "  - Enter as whole number (0-9999)\n";
        cout << "  - Represents maximum length capacity for regular vehicles\n";
        cout << "  - Common values: 100, 150, 200, 300, 500\n";
        cout << "  - Examples: 111 (111 meters), 200 (200 meters)\n";
        cout << "Enter LCLL: ";
        cin >> lcll;

        // reprompts user if entered value is not a float or too big/small.
        // value is converted to DDDD.D form when saved.
        if (cin.fail() || lcll > LCLL_MAX || lcll < 0)
        {
            cout << "Invalid LCLL. Must be a number between 0-9999.\n";
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

    // Loop goal: Keep prompting until user enters a valid HCLL value (0-9999)
    while (!validEntry)
    {
        cout << "\nHigh Clearance Lane Limit (HCLL):\n";
        cout << "Format: Maximum length for special vehicles in meters\n";
        cout << "Guidelines:\n";
        cout << "  - Enter as whole number (0-9999)\n";
        cout << "  - Represents maximum length capacity for special/oversized vehicles\n";
        cout << "  - Usually higher than LCLL\n";
        cout << "  - Common values: 200, 300, 500, 1000\n";
        cout << "  - Examples: 222 (222 meters), 300 (300 meters)\n";
        cout << "Enter HCLL: ";
        cin >> hcll;

        // reprompts user if entered value is not a float or too big/small.
        // value is converted to DDDD.D form when saved.
        if (cin.fail() || hcll > HCLL_MAX || hcll < 0)
        {
            cout << "Invalid HCLL. Must be a number between 0-9999.\n";
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
    // Loop goal: Keep prompting until user enters valid confirmation choice (cancel or confirm)
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

// Helper function to calculate capacity percentage for sailing reports
float Sailing::calculateCapacityPercentage(int totalLow, int totalHigh, float remainingLow, float remainingHigh)
{
    // Calculate percentage full: (total capacity - remaining capacity) / total capacity * 100
    double totalCapacity = (double)(totalLow + totalHigh);
    double remainingCapacity = (double)(remainingLow + remainingHigh);
    
    if (totalCapacity <= 0) {
        return 0.0f; // Avoid division by zero
    }
    
    float percentFull = (float)((totalCapacity - remainingCapacity) / totalCapacity * 100);
    return percentFull;
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
        // Loop goal: Parse all fields from the line separated by '|' delimiter
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

//-----------------------------------------------------------------------------------------
// Static method to edit a sailing by sailing ID with validation
void Sailing::editSailing(const char *sailingID)
{
    // Get the sailing object from the file I/O system
    if (!sailingFileIO::exists(sailingID))
    {
        cout << "Sailing with ID " << sailingID << " does not exist.\n";
        return;
    }
    else
    {
        Sailing s = Sailing::getSailingFromIO(sailingID);
        s.editSailing();
    }
}
