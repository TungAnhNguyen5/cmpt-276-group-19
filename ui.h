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
    void manageReservationsForSailing(const char* sailingID);

    // Additional UI functions made public
    
    // addNewSailing
    // Adds a new sailing to the system with user input
    // out: bool - true if sailing was successfully added, false if cancelled
    bool addNewSailing();
    
    // editSailing
    // Edits an existing sailing with the given ID
    // in: sailingID - the ID of the sailing to edit
    // out: none
    void editSailing(const char* sailingID);
    
    // addNewVehicle
    // Adds a new vehicle to the system with user input
    // out: bool - true if vehicle was successfully added, false if cancelled
    bool addNewVehicle();
    
    // editVehicleDetailed
    // Edits an existing vehicle with the given license plate
    // in: licensePlate - the license plate of the vehicle to edit
    // out: none
    void editVehicleDetailed(const string& licensePlate);
    
    // addNewReservation
    // Adds a new reservation for the specified sailing
    // in: sailingID - the sailing ID to add reservation for
    // out: bool - true if reservation was successfully added, false if cancelled
    bool addNewReservation(const char* sailingID);
    
    // editReservation
    // Edits or deletes a reservation for the specified sailing and license plate
    // in: sailingID - the sailing ID
    // in: licensePlate - the license plate of the reservation
    // out: none
    void editReservation(const char* sailingID, const string& licensePlate);
    
    // checkInProcess
    // Handles the check-in process for reservations
    // out: none
    void checkInProcess();
    
    // showSailingReport
    // Displays a comprehensive sailing report
    // out: none
    void showSailingReport();
    
    // manageSailingsMenu
    // Displays and handles the sailing management submenu
    // out: none
    void manageSailingsMenu();
    
    // manageVehiclesMenu
    // Displays and handles the vehicle management submenu
    // out: none
    void manageVehiclesMenu();
    
    // manageReservationsMenu
    // Displays and handles the reservation management submenu
    // out: none
    void manageReservationsMenu();
    
    // displayHeader
    // Displays a formatted section header
    // in: title - the title to display in the header
    // out: none
    void displayHeader(const string& title);
    
    // displayFooter
    // Displays a formatted section footer
    // out: none
    void displayFooter();
    
    // getValidIntInput
    // Gets valid integer input from user within specified range
    // in: min - minimum allowed value (default 0)
    // in: max - maximum allowed value (default 9)
    // out: int - the valid integer input
    int getValidIntInput(int min = 0, int max = 9);
    
    // getStringInput
    // Gets string input from user with optional cancellation support
    // in: prompt - the prompt to display to user
    // in: allowCancel - whether to allow cancellation (default true)
    // out: string - the user input or "CANCEL" if cancelled
    string getStringInput(const string& prompt, bool allowCancel = true);
    
    // pauseForUser
    // Pauses execution and waits for user to press Enter
    // out: none
    void pauseForUser();

} // namespace UI

#endif // UI_H
