// FILE: UI.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
// Declares the interface for the console-based user interface,
// including initialization, main menu loop, and shutdown.
// Connects Sailing, Vehicle, and Reservation modules through
// a user-friendly text-based interface with step-by-step guidance.
//************************************************************
// USAGE:
// - Call UI::initialize() at program start (e.g., from main.cpp).
// - Call UI::mainMenu() to display options and handle user input.
// - Call UI::shutdown() before program exit.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - James Nguyen
//          - Initial draft for interface specification.
// Rev. 2 - 2025/07/23 - Group 19
//          - Updated for integrated UI with real module integration.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter
//************************************************************

#ifndef UI_H
#define UI_H

#include "sailing.h"
#include "vehicle.h"
#include "vehicleFileIO.h"
#include "reservation.h"
#include <string>

using namespace std;

// Namespace UI: console-based user interface controller
namespace UI {

    // initialize
    // Sets up modules and loads initial data.
    // Initializes sailing and reservation modules.
    // Returns true if successful, false if initialization fails.
    // out: bool - success status
    bool initialize();

    // mainMenu
    // Displays the main menu and processes user commands until exit is selected.
    // Provides access to sailing, vehicle, reservation management, and reports.
    // Features numbered menu navigation with user-friendly prompts.
    // in: none
    // out: none
    void mainMenu();

    // shutdown
    // Saves any pending changes and cleans up resources.
    // Calls shutdown methods for sailing and reservation modules.
    // out: none
    void shutdown();

    // manageReservationsForSailing
    // Manages reservations for a specific sailing ID.
    // Called from sailing module when editing sailings.
    // in: sailingID - the sailing ID to manage reservations for
    // out: none
    void manageReservationsForSailing(const string& sailingID);

} // namespace UI

#endif // UI_H
