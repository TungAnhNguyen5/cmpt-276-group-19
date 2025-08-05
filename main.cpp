// PROJECT: CMPT 276 – Ferry Reservation Software System 
// TEAM: Group 19
// DATE: 2025/07/09
//************************************************************
// PURPOSE:
// Entry point of the Ferry Reservation Software System.
// Initializes the system, runs the main UI loop, and shuts down // cleanly.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/24 - James Nguyen
// - Initial version of main.cpp file.
// Rev. 2 - 2025/08/05 - James Nguyen
// - Finalized main.cpp to integrate with UI and system modules.
// - Ensured proper initialization and shutdown sequence.
//************************************************************


#include <iostream>
#include "ui.h"

int main() {
    // Initialize system modules
    if (!UI::initialize()) {
        std::cerr << "Initialization failed. Exiting program.\n";
        return 1;
    }

    // Launch main menu loop
    UI::mainMenu();

    // Shutdown and cleanup
    UI::shutdown();

    return 0;
}
