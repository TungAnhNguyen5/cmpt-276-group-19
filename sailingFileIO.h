#ifndef SAILING_FILE_IO_H
#define SAILING_FILE_IO_H

#include <fstream>
#include "sailing.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <fcntl.h> 

//-------------------------------------------------------------------------------------------------
// class used to read and write the saved sailings
class sailingFileIO
{
private:
    // the file in which sailing data is saved to.
    static std::fstream file;
    // helper function to create the data file if it does not already exist
    static void createFile();
    // helper function for deleting to get the last one
    static Sailing getLast();
    //helper function for truncating the file
    static void truncateFile();

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