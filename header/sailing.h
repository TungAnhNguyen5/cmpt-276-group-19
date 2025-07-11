//************************************************************
// FILE: Sailing.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #3)
// TEAM: Group 19
// DATE: 2025/07/09
//************************************************************
// PURPOSE:
// Declares the interface for managing sailings, including adding, editing,
// updating lane usage, and displaying reports. Supports interaction with
// persistent sailing records in the system.
//************************************************************
// USAGE:
// - Call initialize() before using any functions in this module.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - Nathan Miller 
//          - Initial draft for interface specification.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter
//************************************************************

#ifndef SAILING_H
#define SAILING_H

#include <string>

//--------------------------------------------------
// Represents a ferry sailing, with vessel info and lane usage tracking.
class Sailing
{
private:
    std::string sailingID; // sailing ID in format: terminal-date-hour
    std::string vesselID;  // vessel name
    float lcll;            // total low ceiling lane length (regular vehicles)
    float hcll;            // total high ceiling lane length (special vehicles)
    float lrl;             // remaining regular lane length
    float hrl;             // remaining special lane length

public:
    //--------------------------------------------------
    // Adds a new sailing by prompting user input. Aborts if duplicate.
    // Returns false if sailing already exists.
    bool addSailing();

    //--------------------------------------------------
    // Opens a sailing record to allow editing or deletion.
    void editSailing();

    //--------------------------------------------------
    // Initializes the Sailing module (e.g., opens file I/O).
    static void initialize();

    //--------------------------------------------------
    // Returns remaining high ceiling lane length.
    // out: remaining special vehicle space
    int hrlRemaining() const;

    //--------------------------------------------------
    // Returns remaining low ceiling lane length.
    // out: remaining regular vehicle space
    int lrlRemaining() const;

    //--------------------------------------------------
    // Finalizes the Sailing module (e.g., closes file I/O).
    static void shutdown();

    //--------------------------------------------------
    // Reduces available high ceiling lane length.
    // in: f – value to subtract from hrl
    void hrlUpdate(float f);

    //--------------------------------------------------
    // Reduces available low ceiling lane length.
    // in: f – value to subtract from lrl
    void lrlUpdate(float f);

    //--------------------------------------------------
    // Displays a report of all sailing data stored.
    static void displayReport();
};

#endif // SAILING_H
