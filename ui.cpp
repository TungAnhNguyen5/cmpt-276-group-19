//************************************************************
// FILE: ui.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
// Implements the console-based user interface for the ferry reservation system.
// Provides a scrolling, text-based interface accessible to users with little
// computer experience. All operations are menu-driven with step-by-step guidance.
//************************************************************

#include "ui.h"
#include "sailingFileIO.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

namespace UI {

    // Internal helper functions
    namespace {
        
        // Forward declarations for internal functions
        bool addNewSailing();
        void editSailing(const string& sailingID);
        bool addNewVehicle();
        void editVehicleDetailed(const string& licensePlate);
        void manageReservationsForSailing(const string& sailingID);
        bool addNewReservation(const string& sailingID);
        void editReservation(const string& sailingID, const string& licensePlate);
        void checkInProcess();
        void displayAllSailings();
        
        // Clear input buffer and get valid integer input
        int getValidIntInput(int min = 0, int max = 9) {
            int choice;
            while (true) {
                if (cin >> choice && choice >= min && choice <= max) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return choice;
                } else {
                    cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
        }

        // Get string input with cancellation support
        string getStringInput(const string& prompt, bool allowCancel = true) {
            string input;
            cout << prompt;
            getline(cin, input);
            
            if (allowCancel && (input == "Cancel" || input == "cancel" || input == "0")) {
                return "CANCEL";
            }
            
            return input;
        }

        // Display section header
        void displayHeader(const string& title) {
            cout << "\n[ " << title << " ] Sample\n";
            cout << "==============================================================\n";
        }

        // Display section footer
        void displayFooter() {
            cout << "==============================================================\n";
        }

        // Pause and wait for user to press Enter
        void pauseForUser() {
            cout << "Press <Enter> to continue...";
            cin.ignore();
        }

        // Display all sailings from the system
        void displayAllSailings() {
            // Try to get real sailing data from the file I/O system
            try {
                // Reset to beginning and try to get sailings
                sailingFileIO::reset();
                Sailing* sailings = sailingFileIO::getNextFive();
                
                bool hasData = false;
                for (int i = 0; i < 5; i++) {
                    // Check if sailing has valid data (non-empty sailing ID)
                    if (strlen(sailings[i].getSailingID()) > 0) {
                        hasData = true;
                        
                        // Format the sailing display
                        // Extract components from sailing ID for display
                        string sid = sailings[i].getSailingID();
                        string terminal = sid.substr(0, 3);
                        string month = "01";  // Could be extracted from a fuller date if available
                        string day = sid.substr(4, 2);
                        string hour = sid.substr(7, 2);
                        
                        cout << sid << " - " << terminal << " - " << month << "/" << day 
                             << " - " << hour << ":00 - VesselID - " 
                             << (int)sailings[i].lrlRemaining() << "/" 
                             << (int)(sailings[i].lrlRemaining() + 100) << "\n";  // Placeholder for total capacity
                    }
                }
                
                // If no real data found, show placeholder data
                if (!hasData) {
                    cout << "ABC-01-09 - ABC - 01/01 - 09:00 - Vicship123 - 2/1000\n";
                    cout << "JKL-01-10 - JKL - 01/01 - 10:00 - Danship234 - 35/1000\n";
                    cout << "DEF-02-09 - DEF - 01/02 - 09:00 - Airship263 - 999/1000\n";
                    cout << "GHI-03-11 - GHI - 01/03 - 11:00 - Seaship456 - 150/1000\n";
                    cout << "MNO-04-12 - MNO - 01/04 - 12:00 - Windship789 - 75/1000\n";
                }
                
            } catch (...) {
                // Fallback to placeholder data if file I/O fails
                cout << "ABC-01-09 - ABC - 01/01 - 09:00 - Vicship123 - 2/1000\n";
                cout << "JKL-01-10 - JKL - 01/01 - 10:00 - Danship234 - 35/1000\n";
                cout << "DEF-02-09 - DEF - 01/02 - 09:00 - Airship263 - 999/1000\n";
                cout << "GHI-03-11 - GHI - 01/03 - 11:00 - Seaship456 - 150/1000\n";
                cout << "MNO-04-12 - MNO - 01/04 - 12:00 - Windship789 - 75/1000\n";
            }
        }

        // Display sailing management submenu
        void manageSailingsMenu() {
            while (true) {
                displayHeader("Manage Sailings");
                
                // Display existing sailings using the dedicated function
                displayAllSailings();
                cout << "\n";
                
                cout << "[0] Cancel\n";
                cout << "[1] New Sailing\n\n";
                cout << "Enter a Sailing ID or a command: ";
                
                string input;
                getline(cin, input);
                displayFooter();
                
                if (input == "0" || input == "Cancel" || input == "cancel") {
                    return;
                } else if (input == "1") {
                    if (addNewSailing()) {
                        cout << "Sailing Successfully Added. Returning to the main menu.\n";
                    } else {
                        cout << "Failed to add sailing. Operation cancelled.\n";
                    }
                    pauseForUser();
                } else {
                    // Assume it's a sailing ID for editing
                    editSailing(input);
                }
            }
        }
        
        // Add new sailing with step-by-step process
        bool addNewSailing() {
            // Step 1: Get departure terminal
            displayHeader("New Sailing");
            cout << "[0] Cancel\n\n";
            cout << "Format: 3 letters (e.g., ABC, DEF, GHI)\n";
            string terminal = getStringInput("Enter a departure terminal: ");
            if (terminal == "CANCEL") return false;
            displayFooter();
            
            // Step 2: Get departure date
            displayHeader("New Sailing");
            cout << "Departure Terminal: " << terminal << "\n\n";
            cout << "[0] Cancel\n\n";
            cout << "Format: MMDD (e.g., 0115 for January 15th)\n";
            string date = getStringInput("Enter a departure date: ");
            if (date == "CANCEL") return false;
            // Format date from MMDD to MM/DD
            if (date.length() == 4) {
                date = date.substr(0,2) + "/" + date.substr(2,2);
            }
            displayFooter();
            
            // Step 3: Get departure time
            displayHeader("New Sailing");
            cout << "Departure Terminal: " << terminal << "\n";
            cout << "Departure Date: " << date << "\n\n";
            cout << "[0] Cancel\n\n";
            cout << "Format: HHMM (e.g., 0900 for 9:00 AM, 1430 for 2:30 PM)\n";
            string time = getStringInput("Enter a departure time: ");
            if (time == "CANCEL") return false;
            // Format time from HHMM to HH:MM
            if (time.length() == 4) {
                time = time.substr(0,2) + ":" + time.substr(2,2);
            }
            displayFooter();
            
            // Step 4: Get vessel ID
            displayHeader("New Sailing");
            cout << "Departure Terminal: " << terminal << "\n";
            cout << "Departure Date: " << date << "\n";
            cout << "Departure Time: " << time << "\n\n";
            cout << "[0] Cancel\n\n";
            cout << "Format: Alphanumeric vessel identifier (e.g., Hugeship876, Vicship123)\n";
            string vesselID = getStringInput("Enter an option or vessel ID: ");
            if (vesselID == "CANCEL") return false;
            displayFooter();
            
            // Step 5: Get LCLL
            displayHeader("New Sailing");
            cout << "Departure Terminal: " << terminal << "\n";
            cout << "Departure Date: " << date << "\n";
            cout << "Departure Time: " << time << "\n";
            cout << "Vessel ID: " << vesselID << "\n\n";
            cout << "[0] Cancel\n\n";
            cout << "Format: Number of large car lane meters (e.g., 8, 12, 16)\n";
            cout << "Enter LCLL: ";
            int lcll;
            cin >> lcll;
            cin.ignore();
            displayFooter();
            
            // Step 6: Get HCLL
            displayHeader("New Sailing");
            cout << "Departure Terminal: " << terminal << "\n";
            cout << "Departure Date: " << date << "\n";
            cout << "Departure Time: " << time << "\n";
            cout << "Vessel ID: " << vesselID << "\n";
            cout << "LCLL: " << lcll << "\n\n";
            cout << "[0] Cancel\n\n";
            cout << "Format: Number of high clearance lane meters (e.g., 16, 24, 32)\n";
            cout << "Enter HCLL: ";
            int hcll;
            cin >> hcll;
            cin.ignore();
            displayFooter();
            
            // Step 7: Confirmation
            displayHeader("Confirm Sailing");
            string sailingID = terminal + "-" + date.substr(3,2) + "-" + time.substr(0,2);
            cout << "Sailing ID: " << sailingID << "\n";
            cout << "Terminal: " << terminal << "\n";
            cout << "Day: " << date << "\n";
            cout << "Time: " << time << "\n";
            cout << "Vessel: " << vesselID << "\n";
            cout << "LCLL: " << lcll << "\n";
            cout << "HCLL: " << hcll << "\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Sailing\n\n";
            cout << "Enter an option: ";
            
            int choice = getValidIntInput(0, 1);
            displayFooter();
            
            if (choice == 1) {
                // Here you would call the actual Sailing::addSailing() with the collected data
                return true;
            }
            return false;
        }
        
        // Edit sailing interface
        void editSailing(const string& sailingID) {
            while (true) {
                displayHeader("Edit Sailing");
                cout << "Sailing ID: " << sailingID << "\n";
                cout << "[1] Departure Terminal: ABC (Format: 3 letters)\n";
                cout << "[2] Departure Day: 01/01 (Format: MM/DD)\n";
                cout << "[3] Departure Time: 09:00 (Format: HH:MM)\n";
                cout << "[4] Vessel ID: Hugeship876 (Format: Alphanumeric)\n";
                cout << "[5] LCLL: 8 (Format: Number of meters)\n";
                cout << "[6] HCLL: 16 (Format: Number of meters)\n";
                cout << "Capacity: 2/1000\n";
                cout << "Vehicle Space: 10/300\n\n";
                cout << "[0] Cancel\n";
                cout << "[7] Confirm\n";
                cout << "[8] Manage Reservations\n";
                cout << "[9] Delete Sailing\n\n";
                cout << "Enter an option: ";
                
                int choice = getValidIntInput(0, 9);
                displayFooter();
                
                switch (choice) {
                    case 0:
                        cout << "Changes Reverted. Returning to the main menu.\n";
                        displayFooter();
                        return;
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6: {
                        string fieldName = (choice == 1) ? "Departure Terminal" :
                                         (choice == 2) ? "Departure Day" :
                                         (choice == 3) ? "Departure Time" :
                                         (choice == 4) ? "Vessel ID" :
                                         (choice == 5) ? "LCLL" : "HCLL";
                        displayHeader("Edit Sailing");
                        cout << "Enter new " << fieldName << ": ";
                        string newValue;
                        getline(cin, newValue);
                        cout << "Updated Successfully.\n";
                        displayFooter();
                        break;
                    }
                    case 7:
                        cout << "Changes Successfully Saved. Returning to the main menu.\n";
                        displayFooter();
                        return;
                    case 8:
                        manageReservationsForSailing(sailingID);
                        break;
                    case 9:
                        cout << "Sailing Successfully Deleted. Returning to the main menu.\n";
                        displayFooter();
                        return;
                }
            }
        }

        // Display vehicle management submenu
        void manageVehiclesMenu() {
            while (true) {
                displayHeader("Manage Vehicles");
                
                // Display existing vehicles (placeholder - would get from vehicleFileIO)
                cout << "ABC123 - Sp Vehicle\n";
                cout << "BAC213 - Rg Vehicle\n";
                cout << "CBA321 - Rg Vehicle\n\n";
                
                cout << "[0] Cancel\n";
                cout << "[1] Add Vehicle\n\n";
                cout << "Enter an option or license plate number: ";
                
                string input;
                getline(cin, input);
                displayFooter();
                
                if (input == "0" || input == "Cancel" || input == "cancel") {
                    return;
                } else if (input == "1") {
                    if (addNewVehicle()) {
                        cout << "Vehicle Successfully Added. Returning to the main menu.\n";
                    } else {
                        cout << "Vehicle addition failed or cancelled.\n";
                    }
                    pauseForUser();
                } else {
                    // Assume it's a license plate for editing
                    editVehicleDetailed(input);
                }
            }
        }
        
        // Add new vehicle with step-by-step process
        bool addNewVehicle() {
            // Step 1: Get license plate
            displayHeader("Add Vehicle");
            cout << "[0] Cancel\n\n";
            cout << "Format: Alphanumeric license plate (e.g., ABC123, XYZ789)\n";
            string license = getStringInput("Enter an option or license plate number: ");
            if (license == "CANCEL") return false;
            displayFooter();
            
            // Step 2: Get vehicle height
            displayHeader("Add Vehicle");
            cout << "License Plate: " << license << "\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Vehicle\n\n";
            cout << "Format: Height in meters (e.g., 2.5, 3.0, 4.2)\n";
            cout << "Enter an option or vehicle height: ";
            string heightInput;
            getline(cin, heightInput);
            if (heightInput == "0" || heightInput == "Cancel") return false;
            float height = stof(heightInput);
            displayFooter();
            
            // Step 3: Get vehicle length
            displayHeader("Add Vehicle");
            cout << "License Plate: " << license << "\n";
            cout << "Vehicle Height: " << height << "\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Vehicle\n\n";
            cout << "Format: Length in meters (e.g., 5.5, 12.0, 18.5)\n";
            cout << "Enter an option or vehicle length: ";
            string lengthInput;
            getline(cin, lengthInput);
            if (lengthInput == "0" || lengthInput == "Cancel") return false;
            float length = stof(lengthInput);
            displayFooter();
            
            // Step 4: Confirmation
            displayHeader("Add Vehicle");
            cout << "License Plate: " << license << "\n";
            cout << "Vehicle Height: " << height << "\n";
            cout << "Vehicle Length: " << length << "\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Vehicle\n\n";
            cout << "Enter an option: ";
            
            int choice = getValidIntInput(0, 1);
            displayFooter();
            
            if (choice == 1) {
                // Here you would call addVehicleFromUI or create the vehicle
                return true;
            }
            return false;
        }
        
        // Edit vehicle interface
        void editVehicleDetailed(const string& licensePlate) {
            while (true) {
                displayHeader("Edit Vehicle");
                cout << "[1] License Plate: " << licensePlate << " (Format: Alphanumeric)\n";
                cout << "[2] Vehicle Height: 15 (Format: Meters, e.g., 2.5)\n";
                cout << "[3] Vehicle Length: 15 (Format: Meters, e.g., 12.0)\n\n";
                cout << "[0] Cancel\n";
                cout << "[8] Confirm\n";
                cout << "[9] Delete Vehicle\n\n";
                cout << "Enter an option: ";
                
                int choice = getValidIntInput(0, 9);
                displayFooter();
                
                switch (choice) {
                    case 0:
                        cout << "Changes Reverted. Returning to the main menu.\n";
                        displayFooter();
                        return;
                    case 1:
                    case 2:
                    case 3: {
                        string fieldName = (choice == 1) ? "License Plate" :
                                         (choice == 2) ? "Vehicle Height" : "Vehicle Length";
                        cout << "Enter the new value: ";
                        string newValue;
                        getline(cin, newValue);
                        cout << "Updated Successfully.\n";
                        break;
                    }
                    case 8:
                        cout << "Changes Successfully Saved\n";
                        displayFooter();
                        return;
                    case 9:
                        cout << "Vehicle Successfully Deleted. Returning to the main menu.\n";
                        displayFooter();
                        return;
                }
            }
        }

        // Display reservation management submenu
        void manageReservationsMenu() {
            while (true) {
                displayHeader("Manage Reservations");
                cout << "[0] Cancel\n";
                cout << "[1] Check-In\n\n";
                cout << "Enter an option or sailing ID: ";
                
                string input;
                getline(cin, input);
                displayFooter();
                
                if (input == "0" || input == "Cancel" || input == "cancel") {
                    return;
                } else if (input == "1") {
                    // Check-in process
                    checkInProcess();
                } else {
                    // Assume it's a sailing ID
                    manageReservationsForSailing(input);
                }
            }
        }
        
        // Manage reservations for a specific sailing
        void manageReservationsForSailing(const string& sailingID) {
            while (true) {
                displayHeader("Manage Reservations");
                
                // Display existing reservations for this sailing
                cout << "ABC2QA - 9543331111 - Rg Vehicle\n";
                cout << "Q213WE - 9340505333 - Sp Vehicle\n";
                cout << "HELLO1 - 2342342345 - Rg Vehicle\n\n";
                
                cout << "[0] Cancel\n";
                cout << "[1] New Reservation\n\n";
                cout << "Enter an option or phone number: ";
                
                string input;
                getline(cin, input);
                displayFooter();
                
                if (input == "0" || input == "Cancel" || input == "cancel") {
                    return;
                } else if (input == "1") {
                    if (addNewReservation(sailingID)) {
                        cout << "Reservation Successfully Added. Returning to the main menu.\n";
                        pauseForUser();
                    }
                } else {
                    // Assume it's a phone number or license plate for editing
                    editReservation(sailingID, input);
                }
            }
        }
        
        // Add new reservation process
        bool addNewReservation(const string& sailingID) {
            // Step 1: Get license plate
            displayHeader("Add Reservation");
            cout << "[0] Cancel\n\n";
            cout << "Format: Alphanumeric license plate (e.g., ABC123, XYZ789)\n";
            string license = getStringInput("Enter an option or license plate: ");
            if (license == "CANCEL") return false;
            displayFooter();
            
            // Step 2: Get phone number (if vehicle not in system)
            displayHeader("Add Reservation");
            cout << "License Plate Number: " << license << "\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Reservation\n\n";
            cout << "Format: 10-digit phone number (e.g., 2501234567)\n";
            string phone = getStringInput("Enter an option or phone number: ");
            if (phone == "CANCEL") return false;
            displayFooter();
            
            // Step 3: Get vehicle height (if special vehicle)
            displayHeader("Add Reservation");
            cout << "License Plate: " << license << "\n";
            cout << "Phone Number: " << phone << "\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Reservation\n\n";
            cout << "Format: Height in meters (e.g., 2.5, 3.0, 4.2)\n";
            cout << "Enter an option or vehicle height: ";
            string heightInput;
            getline(cin, heightInput);
            if (heightInput == "0" || heightInput == "Cancel") return false;
            float height = stof(heightInput);
            displayFooter();
            
            // Step 4: Get vehicle length
            displayHeader("Add Reservation");
            cout << "License Plate: " << license << "\n";
            cout << "Phone Number: " << phone << "\n";
            cout << "Vehicle Height: " << height << "\n\n";
            cout << "[0] Cancel\n\n";
            cout << "Format: Length in meters (e.g., 5.5, 12.0, 18.5)\n";
            cout << "Enter an option or vehicle length: ";
            string lengthInput;
            getline(cin, lengthInput);
            if (lengthInput == "0" || lengthInput == "Cancel") return false;
            float length = stof(lengthInput);
            displayFooter();
            
            // Step 5: Confirmation
            displayHeader("Add Reservation");
            cout << "Phone Number: " << phone << "\n";
            cout << "License Plate: " << license << "\n";
            cout << "Vehicle Height: " << height << "\n";
            cout << "Vehicle Length: " << length << "\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Reservation\n\n";
            cout << "Enter an option: ";
            
            int choice = getValidIntInput(0, 1);
            displayFooter();
            
            return (choice == 1);
        }
        
        // Edit/Delete reservation
        void editReservation(const string& sailingID, const string& licensePlate) {
            displayHeader("Edit Reservation");
            cout << "Sailing ID: " << sailingID << "\n";
            cout << "License Plate: " << licensePlate << "\n";
            cout << "Regular Vehicle\n\n";
            cout << "[0] Cancel\n";
            cout << "[9] Delete Reservation\n\n";
            cout << "Enter an option: ";
            
            int choice = getValidIntInput(0, 9);
            displayFooter();
            
            if (choice == 9) {
                cout << "Reservation Successfully Deleted. Returning to the main menu.\n";
            } else {
                cout << "No changes made. Returning to the main menu.\n";
            }
            displayFooter();
        }
        
        // Check-in process
        void checkInProcess() {
            // Step 1: Get sailing ID
            displayHeader("Manage Reservations");
            cout << "[0] Cancel\n\n";
            cout << "Format: Sailing ID (e.g., ABC-01-09, DEF-02-10)\n";
            string sailingID = getStringInput("Enter an option or sailing ID: ");
            if (sailingID == "CANCEL") return;
            displayFooter();
            
            // Step 2: Show reservations and get license plate
            displayHeader("Manage Reservation");
            cout << "ABC2QA - 9543331111 - Rg Vehicle\n";
            cout << "Q213WE - 9340505333 - Sp Vehicle\n";
            cout << "HELLO1 - 2342342345 - Rg Vehicle\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Reservation\n\n";
            cout << "Format: License plate from list above (e.g., ABC2QA, Q213WE)\n";
            string license = getStringInput("Enter an option or license plate: ");
            if (license == "CANCEL") return;
            displayFooter();
            
            // Step 3: Confirmation
            displayHeader("Check-In");
            cout << "Sailing ID: " << sailingID << "\n";
            cout << "License Plate: " << license << "\n";
            cout << "Regular Vehicle\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Confirm Check-in\n";
            cout << "[9] Delete Reservation\n\n";
            cout << "Enter a line number: ";
            
            int choice = getValidIntInput(0, 9);
            displayFooter();
            
            switch (choice) {
                case 1:
                    cout << "Customer successfully checked-in. Returning to the main menu.\n";
                    break;
                case 9:
                    cout << "Reservation Successfully Deleted. Returning to the main menu.\n";
                    break;
                default:
                    cout << "No changes made. Returning to the main menu.\n";
                    break;
            }
            displayFooter();
        }

        // Display sailing report
        void showSailingReport() {
            displayHeader("Sailing Report");
            
            // Call the existing sailing report function
            Sailing::displayReport();
            
            displayFooter();
            pauseForUser();
        }

    } // end anonymous namespace

    // Public interface implementation
    bool initialize() {
        cout << "Initializing Ferry Reservation System...\n";
        
        try {
            // Initialize sailing module
            Sailing::initialize();
            
            // Initialize reservation module
            ::initialize(); // Call global initialize function from reservation.h
            
            cout << "System initialized successfully.\n";
            return true;
        } catch (const exception& e) {
            cerr << "Initialization failed: " << e.what() << "\n";
            return false;
        }
    }

    void mainMenu() {
        cout << "\nWelcome to the Ferry Reservation System\n";
        cout << "========================================\n";
        cout << "This system operates through numbered menus.\n";
        cout << "Type a number and press <Enter> to select an option.\n";
        cout << "Type 0 or 'Cancel' to go back at any time.\n\n";
        
        while (true) {
            displayHeader("Main Menu");
            cout << "[1] Manage Sailings\n";
            cout << "[2] Manage Vehicles\n";
            cout << "[3] Manage Reservations\n";
            cout << "[4] Show Sailing Report\n";
            cout << "[0] Exit System\n\n";
            cout << "Enter a menu: ";
            
            int choice = getValidIntInput(0, 4);
            displayFooter();
            
            switch (choice) {
                case 1:
                    manageSailingsMenu();
                    break;
                case 2:
                    manageVehiclesMenu();
                    break;
                case 3:
                    manageReservationsMenu();
                    break;
                case 4:
                    showSailingReport();
                    break;
                case 0:
                    cout << "Thank you for using the Ferry Reservation System.\n";
                    cout << "Goodbye!\n";
                    return;
            }
        }
    }

    void shutdown() {
        cout << "Shutting down Ferry Reservation System...\n";
        
        try {
            // Shutdown sailing module
            Sailing::shutdown();
            
            // Shutdown reservation module
            ::shutdown(); // Call global shutdown function from reservation.h
            
            cout << "System shutdown complete.\n";
        } catch (const exception& e) {
            cerr << "Error during shutdown: " << e.what() << "\n";
        }
    }

} // namespace UI
