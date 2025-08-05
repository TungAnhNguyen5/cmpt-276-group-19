// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/24
//************************************************************
// PURPOSE:
//   Declares the interface for file I/O operations on Sailing //records,
//   including opening, closing, reading, writing, and deleting //sailings.
//   Provides functionality for managing sailing records in a //binary file format.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - Nathan Miller
//          - Initial sailingFileIO header file.
// Rev. 2 - 2025/07/24 - Nathan Miller
//          - Finalized format and synced with .cpp logic.
// Rev. 3 - 2025/08/05 - Nathan Miller
//          - Fully debugged final release version.
//************************************************************

#ifndef SAILING_FILE_IO_H
#define SAILING_FILE_IO_H

#include <fstream>
#include "sailing.h"
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

//-------------------------------------------------------------------------------------------------
// class used to read and write the saved sailings in text format
class sailingFileIO
{
private:
    // the file in which sailing data is saved to.
    static std::fstream file;
    // helper function to create the data file if it does not already exist
    static void createFile();
    // helper function for deleting to get the last one
    static Sailing getLast();
    // helper function for truncating the file
    static void truncateFile();
    // helper function to convert sailing to text line
    static std::string sailingToString(const Sailing& sailing);
    // helper function to parse sailing from text line
    static Sailing parseSailingLine(const std::string& line);

public:
    
    //-----------------------------------------------------------------------------------------
    // closes the file saved, returns false if it is unable to
    static bool closeFile();

    //-----------------------------------------------------------------------------------------
    // opens the file saved, returns false if it is unable to
    static bool openFile();

    //-----------------------------------------------------------------------------------------
    // resets the file reading, used when displaying a sailing report
    static void reset();

    //-----------------------------------------------------------------------------------------
    // checks if the passed sailingID is saved in the database, returns true if it is
    static bool exists(
        const char* sid);

    //-----------------------------------------------------------------------------------------
    // returns the specified sailing from the ID, use exists() to check if it exists first
    static Sailing getSailing(
        const char* sid);

    //-----------------------------------------------------------------------------------------
    // returns up to the next five sailings saved in the database, used for displaying reports.
    static Sailing *getNextFive();

    //-----------------------------------------------------------------------------------------
    // enters the specified sailing into the database, returns true if it works.
    // also handles updates when needed
    static bool saveSailing(const Sailing s);

    //-----------------------------------------------------------------------------------------
    // removes the specified sailing from ID into the database, returns true if it work.
    static bool deleteSailing(const char* sid);
};

#endif