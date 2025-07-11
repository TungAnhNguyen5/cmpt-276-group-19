#ifndef SAILING_FILE_IO_H
#define SAILING_FILE_IO_H

#include <fstream>
#include "sailing.h"
#include <string>

//-------------------------------------------------------------------------------------------------
// static class used to navigate through the saved sailings
static class FileIOforSailing
{
private:
    // the file in which sailing data is saved to.
    iFile data;

public:
    //-----------------------------------------------------------------------------------------
    // closes the file saved, returns false if it is unable to
    bool close();

    //-----------------------------------------------------------------------------------------
    // opens the file saved, returns false if it is unable to
    bool open();

    //-----------------------------------------------------------------------------------------
    // resets the file reading, used when displaying a sailing report
    void reset();

    //-----------------------------------------------------------------------------------------
    // checks if the passed sailingID is saved in the database, returns true if it is
    bool exists(
        std::string sid);

    //-----------------------------------------------------------------------------------------
    // returns the specified sailing from the ID, use exists() to check if it exists first
    Sailing getSailing(
        std::string sid);

    //-----------------------------------------------------------------------------------------
    // returns up to the next five sailings saved in the database, used for displaying reports.
    Sailing *getNextFive();

    //-----------------------------------------------------------------------------------------
    // enters the specified sailing into the database, returns true if it works.
    bool saveSailing(Sailing s);

    //-----------------------------------------------------------------------------------------
    // removes the specified sailing from ID into the database, returns true if it work.
    bool deleteSailing(std::string sid);
}

#endif