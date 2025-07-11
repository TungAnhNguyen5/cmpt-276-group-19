//************************************************************
// FILE: SailingFileIO.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #3)
// TEAM: Group 19
// DATE: 2025/07/09
//************************************************************
// PURPOSE:
// Declares the interface for file I/O operations on Sailing records,
// including opening, closing, querying, and modifying the persistent data.
//************************************************************
// USAGE:
// - Call open() before any other operations.
// - Use exists(), getSailing(), getNextFive() for queries.
// - Use saveSailing(), deleteSailing() for modifications.
// - Call close() when done.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - James Nguyen
//          - Initial draft for interface specification.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter
//************************************************************

#ifndef SAILING_FILE_IO_H
#define SAILING_FILE_IO_H

#include <fstream>
#include "sailing.h"
#include <string>

//--------------------------------------------------
// Static helper for navigating persistent Sailing records.
class FileIOforSailing
{
private:
    std::fstream data;  // fstream object for sailing data file

public:
    //--------------------------------------------------
    // Opens the sailing data file.
    // Returns false if unable to open.
    bool open();

    //--------------------------------------------------
    // Closes the sailing data file.
    // Returns false if unable to close.
    bool close();

    //--------------------------------------------------
    // Resets file read position to beginning.
    void reset();

    //--------------------------------------------------
    // Checks if a sailing with the given ID exists.
    // in:  sid – sailing ID to query
    // Returns true if found.
    bool exists(const std::string &sid);

    //--------------------------------------------------
    // Retrieves a sailing by ID. Use exists() first.
    // in:  sid – sailing ID to retrieve
    // Returns a Sailing object populated from file.
    Sailing getSailing(const std::string &sid);

    //--------------------------------------------------
    // Retrieves up to the next five sailings in file order.
    // Returns pointer to an array of up to five Sailing objects.
    Sailing* getNextFive();

    //--------------------------------------------------
    // Saves a new sailing record to file.
    // in:  s – Sailing object to persist
    // Returns true if successful.
    bool saveSailing(const Sailing &s);

    //--------------------------------------------------
    // Deletes a sailing record by ID.
    // in:  sid – sailing ID to remove
    // Returns true if successful.
    bool deleteSailing(const std::string &sid);
};

#endif // SAILING_FILE_IO_H
