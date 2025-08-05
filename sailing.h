#ifndef SAILING_H
#define SAILING_H

#include <string>
using namespace std;

//-----------------------------------------------------------------------------------------
// Sailing class used to interface with the file io for sailing class as well as create sailings.
// Also used to navigate through the manage sailings menu.
class Sailing
{
private:
    // sailingID is used to identify sailings. Formatted terminal-date-hour
    char sailingID[10];
    // vesselID represents the nme of the vessel used for the sailing
    char vesselID[26];
    // the total amount of space for regular vehicles on the vessel.
    int lcll;
    // the total amount of space for special vehicles on the vessel.
    int hcll;
    // the remaining space for regular vehicles on the vessel.
    float lrl;
    // the remaining space for special vehicles on the vessel.
    float hrl;

    // helper function to handle the departure terminal entry for add sailing
    static string addDepTerm();
    // helper function to handle the date entry for add sailing
    static string addDate();
    // helper function to handle the time entry for add sailing
    static string addTime();
    // helper function to handle the vesselID entry for add sailing
    static string addVessel();
    // helper function to handle the LCLL entry for add sailing
    static int addLCLL();
    // helper function to handle the HCLL entry for add sailing
    static int addHCLL();
    // helper function to prompt the user to confirm the operation they are doing
    static bool confirm(int confirmInput);

public:
    //-----------------------------------------------------------------------------------------
    // initializes a sailing object according to user input, returns false if already in database
    static bool addSailing();

    // creates a sailing using a line in the data file
    void createSailing(string line);

    //-----------------------------------------------------------------------------------------
    // opens a sailing to be deleted and/or view the information saved on it
    void editSailing();

    //-----------------------------------------------------------------------------------------
    // initializes this class as well as opens File I/O for Sailing class
    static void initialize();

    //-----------------------------------------------------------------------------------------
    // getter function for the sailing ID
    const char *getSailingID() const;

    //-----------------------------------------------------------------------------------------
    // used by other classes to access sailings from sailingFileIO
    static Sailing getSailingFromIO(const char *sid);

    //-----------------------------------------------------------------------------------------
    // returns the value of the hrl variable
    float hrlRemaining() const;

    //-----------------------------------------------------------------------------------------
    // returns the value of the lrl variable
    float lrlRemaining() const;

    //-----------------------------------------------------------------------------------------
    // shuts down this class and closes File I/O for Sailing Class
    static void shutdown();

    //-----------------------------------------------------------------------------------------
    // decrements the hrl by the specified value, returns false if unable to
    bool hrlUpdate(
        float f);

    //-----------------------------------------------------------------------------------------
    // decrements lrl by the specified value, returns false if unable to
    bool lrlUpdate(
        float f);

    //-----------------------------------------------------------------------------------------
    // interfaces with File I/O for Sailing Class to run the display report menu
    static void displayReport();
};

#endif