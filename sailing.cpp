#include "sailing.h"
#include "sailingFileIO.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <limits>

using namespace std;

// cancel is always 0 so using a constant for it makes sense
const int CANCEL_OPTION = 0;

// public functions
bool Sailing::addSailing()
{
    const char SID_SEPARATOR = '-';
    const int CONFIRM_OPTION = 1;

    // uses helper functions to prompt the user for and create the sailing id
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
        return false;
    }

    // uses helper functions to prompt the user for the rest of the sailing information.
    string vessel = addVessel();
    int lcll = addLCLL();
    int hcll = addHCLL();

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

    sailingFileIO::saveSailing(s);
    return true;
}

void Sailing::editSailing()
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

    Sailing s = *this;
    string sid = s.sailingID;
    // for loop to print the option select in order
    for (int i = 1; i <= NUM_OF_OPTIONS; i++)
    {
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
    bool editing = true;
    int input;

    // runs the editing loop until either confirm, cancel, delete, or manage reservations is chosen
    while (editing)
    {
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
                break;
            }
            case DEP_DAY:
            {
                string newDepDay;
                newDepDay = addDate();
                const char *newDepDayC = newDepDay.c_str();
                s.sailingID[4] = newDepDayC[0];
                s.sailingID[5] = newDepDayC[1];
                break;
            }
            case DEP_TIME:
            {
                string newDepTime;
                newDepTime = addTime();
                const char *newDepTimeC = newDepTime.c_str();
                s.sailingID[7] = newDepTimeC[0];
                s.sailingID[8] = newDepTimeC[1];
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
                cout << "Sailing successfully edited. Returning to main menu.\n";
                break;
            }
            // opens manage reservations for this sailing, requires reservation class
            case RESERVATIONS_OPTION:
            {
                break;
            }
            case DELETE_OPTION:
            {
                editing = false;
                sailingFileIO::deleteSailing(s.sailingID);
                cout << "Sailing successfully deleted. Returning to main menu.\n";
                break;
            }
            }
        }
    }
}

void Sailing::displayReport()
{
    // initializes variables required for the for loop
    bool reportActive = true;
    const int VESSEL_ID_LENGTH = 25;
    const int LRL_LENGTH = 6;
    const int HRL_LENGTH = 6;
    const int VEHICLE_NUM_LENGTH = 7;
    const int PERCENT_LENGTH = 4;
    const int MAX_VEHICLES = 300;

    // Display report header with current date
    cout << "Sailing Report                                                     25/06/24\n";
    cout << "===============================================================================\n";
    cout << "Sailing ID    Vessel ID               LRL    HRL    # of Vehicles  Percent Full\n";
    cout << "===============================================================================\n";

    // resets position in the file so it starts at the beginning every time
    sailingFileIO::reset();

    while (reportActive)
    {
        // Placeholder data - in real implementation would use sailingFileIO::getNextFive()
        // For now, display sample data that matches the format in the attachment
        cout << "1]ABC-01-09   Brokenship912           30     30     020/300        091%\n";
        cout << "2]ABC-01-17   Hugeship376             05     10     025/300        023%\n";
        cout << "3]ABC-01-12   Brokenship912           20     13     045/300        024%\n";
        cout << "4]ABC-03-06   Hugeship376             11     11     200/300        067%\n";
        cout << "5]EFE-03-06   Brokenship912           50     40     050/300        090%\n";
        cout << "6]CCC-DD-DD   CCCCCCCCCCCCCCCCCCCCCCC DDDD.D DDDD.D DDD/DDD        DDD%\n";

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
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else
            {
                validInput = true;
                if (input == CANCEL_OPTION)
                {
                    reportActive = false;
                }
                // If input is 5, loop continues to show "next 5" (same data for now)
            }
        }
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

// functions for updating the lrl and hrl of a specific sailing
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
        if (date.length() != DATE_LENGTH || stoi(date) < 0 || stoi(date) > 31)
        {
            cout << "Invalid departure date.\n";
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
        if (time.length() != TIME_LENGTH || stoi(time) < 0 || stoi(time) > 23)
        {
            cout << "Invalid departure time.\n";
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
        cout << "[" << confirmOption << "] Confirm\n";
        cout << "Enter an option: ";
        cin >> input;
        if (cin.fail() || (input != CANCEL_OPTION && input != confirmOption))
        {
            cout << "Invalid option.\n";
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