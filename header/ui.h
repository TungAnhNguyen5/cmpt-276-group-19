//************************************************************
// FILE: UI.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #3)
// TEAM: Group 19
// DATE: 2025/07/09
//************************************************************
// PURPOSE:
// Declares the interface for the console-based user interface,
// including initialization, main menu loop, and shutdown.
// Connects Sailing, Vehicle, and Reservation modules.
//************************************************************
// USAGE:
// - Call UI::initialize() at program start (e.g., from main.cpp).
// - Call UI::mainMenu() to display options and handle user input.
// - Call UI::shutdown() before program exit.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - James Nguyen
//          - Initial draft for interface specification.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter
//************************************************************

#ifndef UI_H
#define UI_H

#include "sailing.h"
#include "sailingFileIO.h"
#include "vehicle.h"
#include "vehicleFileIO.h"
#include "reservation.h"
#include "reservationFileIO.h"

//--------------------------------------------------
// Namespace UI: console-based user interface controller
//--------------------------------------------------
namespace UI {

    //--------------------------------------------------
    // initialize
    // Sets up modules and loads initial data.
    // Returns true if successful.
    // out: none
    //--------------------------------------------------
    bool initialize();

    //--------------------------------------------------
    // mainMenu
    // Displays the main menu, processes user commands until exit selected.
    // in: none
    // out: none
    //--------------------------------------------------
    void mainMenu();

    //--------------------------------------------------
    // shutdown
    // Saves any pending changes, cleans up resources.
    // out: none
    //--------------------------------------------------
    void shutdown();

} // namespace UI

#endif // UI_H
