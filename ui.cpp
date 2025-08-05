// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
// Implements the console-based user interface for the ferry 
// reservation system.
// Provides a scrolling, text-based interface accessible to 
// users with little
// computer experience. All operations are menu-driven with 
// step-by-step guidance.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/23 - James Nguyen
//          - Initial implementation for the user interface
// Rev. 2 - 2025/08/05 - James Nguyen
//          - Finalized interface and aligned with .cpp
// implementation.
//************************************************************


#include "ui.h"
#include "sailing.h"
#include "sailingFileIO.h"
#include "vehicle.h"
#include "vehicleFileIO.h"
#include "reservation.h"
#include "reservationFileIO.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cctype>

using namespace std;

namespace UI {

    // Validation helper functions
    bool isValidSailingID(const string &id)
    {
        if (id.length() != 9)
            return false;
        return isalpha(id[0]) && isalpha(id[1]) && isalpha(id[2]) &&
               id[3] == '-' &&
               isdigit(id[4]) && isdigit(id[5]) &&
               id[6] == '-' &&
               isdigit(id[7]) && isdigit(id[8]);
    }

    bool isValidLicensePlate(const std::string &plate)
    {
        if (plate.length() == 0 || plate.length() > 10)
            return false;
        // Loop goal: Check that every character in license plate is alphanumeric
        for (char c : plate)
        {
            if (!isalnum(c))
                return false;
        }
        return true;
    }

    bool isValidPhoneNumber(const std::string &phone)
    {
        if (phone.length() != 10)
            return false;
        // Loop goal: Check that every character in phone number is a digit
        for (char c : phone)
        {
            if (!isdigit(c))
                return false;
        }
        return true;
    }
    
    bool isValidOneDecimalFloat(const std::string &input)
    {
        size_t dot = input.find('.');
        if (dot == std::string::npos)
            return false; // must contain a dot
        if (dot == 0 || dot == input.length() - 1)
            return false; // must have digit before and after

        std::string before = input.substr(0, dot);
        std::string after = input.substr(dot + 1);

        if (after.length() != 1)
            return false; // must be exactly 1 digit after the decimal

        // Loop goal: Check that all characters before and after decimal point 
        //are digits
        for (char c : before + after)
        {
            if (!isdigit(c))
                return false;
        }

        return true;
    }

    // Implementation of UI functions
    
    // Clear input buffer and get valid integer input
    int getValidIntInput(int min, int max) {
        int choice;
        // Loop goal: Keep prompting until user enters a valid integer within 
        // the specified range
        while (true) {
            if (cin >> choice && choice >= min && choice <= max) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            } else if (cin.eof()) {
                // Handle end of input (e.g., when using piped input)
                return 0; // Default to cancel/exit option
            } else {
                cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    // Get string input with cancellation support
    string getStringInput(const string& prompt, bool allowCancel) {
        string input;
        cout << prompt;
        if (!getline(cin, input)) {
            // Handle EOF or input stream error
            return allowCancel ? "CANCEL" : "";
        }
        
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
    }    // Display sailing management submenu
    void manageSailingsMenu() {
        // Loop goal: Keep displaying sailing management menu until user exits
        while (true) {
            displayHeader("Manage Sailings");
            
            cout << "[0] Cancel\n";
            cout << "[1] New Sailing\n\n";
            cout << "Format: Sailing ID (e.g., TSA-15-08, HGB-01-14, SWB-28-23)\n";
            cout << "Enter a Sailing ID or a command: ";
            
            string input;
            if (!getline(cin, input)) {
                return; // Handle EOF
            }
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
            } else if (input.empty()) {
                // Handle empty input - just continue to show menu again
                continue;
            } else {
                // Assume it's a sailing ID for editing
                editSailing(input.c_str());
            }
        }
    }
    
    // Add new sailing with step-by-step process
    bool addNewSailing() {
        // Call the existing Sailing::addSailing() function
        return Sailing::addSailing();
    }
    
    // Edit sailing interface
    void editSailing(const char* sailingID) {
        // Use the static Sailing::editSailing method which includes validation
        Sailing::editSailing(sailingID);
    }

    // Display vehicle management submenu
    void manageVehiclesMenu() {
        // Loop goal: Keep displaying vehicle management menu until user exits
        while (true) {
            displayHeader("Manage Vehicles");
            
            // Display existing vehicles from the file I/O system
            try {
                FileIOforVehicle vehicleIO;
                if (vehicleIO.open()) {
                    vector<Vehicle> vehicles = vehicleIO.getAllVehicles();
                    
                    if (!vehicles.empty()) {
                        int specialCount = 0, regularCount = 0;
                        // Loop goal: Count and display all vehicles in the system
                        for (const auto& vehicle : vehicles) {
                            if (vehicle.isSpecial()) {
                                specialCount++;
                            } else {
                                regularCount++;
                            }
                        }
                        cout << "Total Vehicles: " << vehicles.size() << " (" 
                             << specialCount << " Special, " << regularCount << " Regular)\n";
                        cout << "Enter a license plate number to edit a specific vehicle.\n";
                    } else {
                        cout << "No vehicles found in the system.\n";
                    }
                    vehicleIO.close();
                } else {
                    cout << "Unable to access vehicle data.\n";
                }
            } catch (...) {
                cout << "Error accessing vehicle data.\n";
            }
            cout << "\n";
            
            cout << "[0] Cancel\n";
            cout << "[1] Add Vehicle\n\n";
            cout << "Format: License plate to edit (e.g., ABC123, BC1234, XYZ789)\n";
            cout << "Enter an option or license plate number: ";
            
            string input;
            if (!getline(cin, input)) {
                return; // Handle EOF
            }
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
        // Call the existing addVehicleFromUI() function
        return addVehicleFromUI();
    }
    
    // Edit vehicle interface
    void editVehicleDetailed(const string& licensePlate) {
        // Call the existing editVehicleFromUI() function
        if (editVehicleFromUI(licensePlate)) {
            cout << "Vehicle successfully updated.\n";
        } else {
            cout << "Vehicle edit cancelled or failed.\n";
        }
        displayFooter();
    }

    // Display reservation management submenu
    void manageReservationsMenu()
    {
        // Loop goal: Keep displaying reservation management menu 
        // until user exits
        while (true)
        {
            displayHeader("Manage Reservations");
            cout << "[0] Cancel\n";
            cout << "[1] Check-In\n\n";
            cout << "Enter an option or sailing ID: ";

            string input;
            if (!getline(cin, input))
                return;
            displayFooter();

            if (input == "0" || input == "Cancel" || input == "cancel")
                return;
            else if (input == "1")
            {
                checkInProcess();
            }
            else if (input.empty())
            {
                // Handle empty input - just continue to show menu again
                continue;
            }
            else if (isValidSailingID(input))
            {
                manageReservationsForSailing(input.c_str());
            }
            else
            {
                cout << "Invalid Sailing ID format. Must be AAA-NN-NN (e.g., ABC-01-23).\n";
                pauseForUser();
            }
        }
    }
    
    // Add new reservation process
    bool addNewReservation(const char *sailingID)
    {
        string license;
        do
        {
            displayHeader("Add Reservation");
            cout << "[0] Cancel\n\n";
            cout << "Format: Alphanumeric license plate (max 10 characters)\n";
            license = getStringInput("Enter an option or license plate: ");
            displayFooter();

            if (license == "CANCEL")
                return false;
            if (!isValidLicensePlate(license))
            {
                cout << "Invalid license plate. Must be alphanumeric and ≤ 10 characters.\n";
                pauseForUser();
            }
        } while (!isValidLicensePlate(license));

        string phone;
        do
        {
            displayHeader("Add Reservation");
            cout << "License Plate Number: " << license << "\n\n";
            cout << "[0] Cancel\n\n";
            cout << "Format: 10-digit phone number (e.g., 2501234567)\n";
            phone = getStringInput("Enter an option or phone number: ");
            displayFooter();

            if (phone == "CANCEL")
                return false;
            if (!isValidPhoneNumber(phone))
            {
                cout << "Invalid phone number. Must be exactly 10 digits.\n";
                pauseForUser();
            }
        } while (!isValidPhoneNumber(phone));

        bool vehicleExists = false;
        float height = 0.0f, length = 0.0f;
        bool isSpecial = false;

        try
        {
            FileIOforVehicle vehicleIO;
            if (vehicleIO.open() && vehicleIO.exists(license))
            {
                Vehicle vehicle = vehicleIO.getVehicle(license);
                vehicleExists = true;
                isSpecial = vehicle.isSpecial();
                height = vehicle.getHeight();
                length = vehicle.getLength();
            }
        }
        catch (...)
        {
        }

        if (!vehicleExists)
        {
            // Prompt for height
            string heightStr;
            do
            {
                displayHeader("Add Reservation");
                cout << "License Plate: " << license << "\n";
                cout << "Phone Number: " << phone << "\n\n";
                cout << "[0] Cancel\n\n";
                cout << "Format: Height in meters (e.g., 2.5, 3.0, 4.2)\n";
                heightStr = getStringInput("Enter an option or vehicle height: ", true);
                displayFooter();

                if (heightStr == "CANCEL")
                    return false;
                if (!isValidOneDecimalFloat(heightStr))
                {
                    cout << "Invalid height. Must be a number with exactly 1 decimal place.\n";
                    pauseForUser();
                }
            } while (!isValidOneDecimalFloat(heightStr));
            height = stof(heightStr);

            string lengthStr;
            do
            {
                displayHeader("Add Reservation");
                cout << "License Plate: " << license << "\n";
                cout << "Phone Number: " << phone << "\n";
                cout << "Vehicle Height: " << fixed << setprecision(1) << height << "\n\n";
                cout << "[0] Cancel\n\n";
                cout << "Format: Length in meters (e.g., 5.5, 12.0, 18.5)\n";
                lengthStr = getStringInput("Enter an option or vehicle length: ", true);
                displayFooter();

                if (lengthStr == "CANCEL")
                    return false;
                if (!isValidOneDecimalFloat(lengthStr))
                {
                    cout << "Invalid length. Must be a number with exactly 1 decimal place.\n";
                    pauseForUser();
                }
            } while (!isValidOneDecimalFloat(lengthStr));
            length = stof(lengthStr);

            // Determine if special based on rules
            isSpecial = (height > 2.0f || length > 7.0f);

            // Add to database
            try
            {
                Vehicle newVehicle;
                newVehicle.addVehicle(license.c_str(), phone.c_str(), length, height);
            }
            catch (...)
            {
            }
        }

        displayHeader("Add Reservation");
        cout << "Phone Number: " << phone << "\n";
        cout << "License Plate: " << license << "\n";
        cout << "Vehicle Height: " << height << "\n";
        cout << "Vehicle Length: " << length << "\n\n";
        cout << "[0] Cancel\n[1] Add Reservation\n\n";
        cout << "Enter an option: ";

        int choice = getValidIntInput(0, 1);
        displayFooter();

        if (choice == 1)
        {
            try
            {
                return addReservation(string(sailingID), license, phone, isSpecial, height, length);
            }
            catch (...)
            {
                return true;
            }
        }
        return false;
    }
    
    // Edit/Delete reservation
    void editReservation(const char *sailingID, const string &licensePlate)
    {
        displayHeader("Edit Reservation");
        cout << "Sailing ID: " << sailingID << "\n";
        cout << "License Plate: " << licensePlate << "\n";
        try
        {
            FileIOforVehicle vehicleIO;
            if (vehicleIO.open() && vehicleIO.exists(licensePlate))
            {
                Vehicle v = vehicleIO.getVehicle(licensePlate);
                cout << (v.isSpecial() ? "Special Vehicle" : "Regular Vehicle") << "\n\n";
                vehicleIO.close();
            }
            else
            {
                cout << "Regular Vehicle\n\n"; // fallback
            }
        }
        catch (...)
        {
            cout << "Regular Vehicle\n\n"; // fallback in case of error
        }
    while (true)
    {
        cout << "[0] Cancel\n[9] Delete Reservation\n\n";
        cout << "Enter an option: ";
        int choice = getValidIntInput(0, 9);
        displayFooter();

        if (choice == 0)
        {
            cout << "No changes made. Returning to the previous menu.\n";
            break;
        }
        else if (choice == 9)
        {
            // Before deleting, determine if it's a special vehicle to restore capacity correctly
            bool isSpecial = false;
            try {
                FileIOforVehicle vehicleIO;
                if (vehicleIO.open() && vehicleIO.exists(licensePlate)) {
                    Vehicle vehicle = vehicleIO.getVehicle(licensePlate);
                    isSpecial = vehicle.isSpecial();
                    vehicleIO.close();
                }
            } catch (...) {
                isSpecial = false; // Default to regular vehicle
            }
            
            if (deleteReservation(licensePlate, sailingID))
            {
                // Restore sailing capacity after successful deletion
                try {
                    Sailing sailing = Sailing::getSailingFromIO(sailingID);
                    if (isSpecial) {
                        // Restore 1 unit of HRL for special vehicle  
                        sailing.hrlUpdate(-1.0f); // Negative to add back capacity
                    } else {
                        // Restore 1 unit of LRL for regular vehicle
                        sailing.lrlUpdate(-1.0f); // Negative to add back capacity
                    }
                    // Save the updated sailing capacity to file
                    sailingFileIO::saveSailing(sailing);
                } catch (...) {
                    // Capacity restoration failed, but deletion succeeded
                }
                
                cout << "Reservation Successfully Deleted.\nReturning to the previous menu.\n";
            }
            else
                cout << "Error: Could not delete reservation.\n";
            break;
        }
        else
        {
            cout << "Invalid option. Please enter 0 or 9.\n";
            pauseForUser(); // optional: add pause between retries
        }
    }

    displayFooter();
}    // Check-in process
    void checkInProcess()
{
    displayHeader("Manage Reservations");
    cout << "[0] Cancel\n\n";
    cout << "Format: Sailing ID (e.g., ABC-01-09, DEF-02-10)\n";
    string sailingID = getStringInput("Enter an option or sailing ID: ");
    if (sailingID == "CANCEL")
        return;
    displayFooter();

    vector<ReservationRecord> records = getAllOnSailing(sailingID);

    // Check if any non-checked-in reservations exist
    bool hasValid = false;
    for (const auto &r : records) {
        if (!r.onboard) {
            hasValid = true;
            break;
        }
    }

    if (!hasValid) {
        cout << "No reservations found for this sailing, \nor all are already checked-in.\n";
        pauseForUser();
        return;
    }

    displayHeader("Manage Reservation");

    FileIOforVehicle vehicleIO;
    vehicleIO.open();

    for (const auto &r : records)
    {
        if (!r.onboard)
        {
            string phone = "Unknown";
            string type = "Rg Vehicle";

            try
            {
                if (vehicleIO.exists(r.licensePlate))
                {
                    Vehicle v = vehicleIO.getVehicle(r.licensePlate);
                    phone = v.getPhone();
                    if (v.isSpecial())
                        type = "Sp Vehicle";
                }
            }
            catch (...) {}

            cout << r.licensePlate << " - " << phone << " - " << type << "\n";
        }
    }

    vehicleIO.close();

    cout << "\n[0] Cancel\n\n";
    string license = getStringInput("Enter an option or license plate: ");
    if (license == "CANCEL")
        return;
    displayFooter();

    // Ensure license is part of an existing non-onboard reservation
    bool matchFound = false;
    for (const auto &r : records) {
        if (r.licensePlate == license && !r.onboard) {
            matchFound = true;
            break;
        }
    }

    if (!matchFound) {
        cout << "License plate not found or already checked-in. \nReturning to the previous menu.\n";
        pauseForUser();
        return;
    }

    displayHeader("Check-In");
    cout << "Sailing ID: " << sailingID << "\n";
    cout << "License Plate: " << license << "\n";

    bool isSpecial = false;
    float height = 0.0f, length = 0.0f;

    try
    {
        if (vehicleIO.open() && vehicleIO.exists(license))
        {
            Vehicle v = vehicleIO.getVehicle(license);
            isSpecial = v.isSpecial();
            height = v.getHeight();
            length = v.getLength();
            cout << (isSpecial ? "Special Vehicle" : "Regular Vehicle") << "\n\n";
            vehicleIO.close();
        }
        else {
            cout << "Regular Vehicle\n\n"; // fallback
        }
    }
    catch (...)
    {
        cout << "Regular Vehicle\n\n"; // fallback
    }

    cout << "[0] Cancel\n[1] Confirm Check-in\n\n";
    cout << "Enter a line number: ";

    int choice = getValidIntInput(0, 9);
    displayFooter();

    switch (choice)
    {
    case 1:
    {
        float fare = checkIn(sailingID, license, isSpecial, height, length);

        if (fare < 0)
            cout << "Reservation not found. Check-in failed.\n";
        else
            cout << "Customer successfully checked-in. Fare: $" << fixed << setprecision(2) << fare << ".\nReturning to the previous menu.\n";
        break;
    }
    default:
        cout << "No changes made. Returning to the previous menu.\n";
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

    // Public interface implementation
    
    // Manage reservations for a specific sailing
    void manageReservationsForSailing(const char* sailingID) {
        while (true) {
            displayHeader("Manage Reservations");
            
            // Display existing reservations for this sailing from actual data
            try {
                vector<ReservationRecord> reservations = getAllOnSailing(sailingID);
                cout << "Reservations for Sailing " << sailingID << ":\n";
                
                if (reservations.empty()) {
                    cout << "  No reservations found for this sailing.\n\n";
                } else {
                    cout << "  License Plate    Phone Number     Status\n";
                    cout << "  ---------------  ---------------  ----------\n";
                    
                    FileIOforVehicle vehicleIO;
                    bool vehicleIOOpen = vehicleIO.open();
                    
                    for (const auto& res : reservations) {
                        string phoneNumber = "N/A";
                        
                        // Get vehicle details for phone number
                        if (vehicleIOOpen && vehicleIO.exists(res.licensePlate)) {
                            Vehicle vehicle = vehicleIO.getVehicle(res.licensePlate);
                            phoneNumber = vehicle.getPhone();
                        }
                        
                        cout << "  " << left << setw(15) << res.licensePlate 
                             << "  " << setw(15) << phoneNumber
                             << "  " << (res.onboard ? "On Board" : "Reserved") << "\n";
                    }
                    
                    if (vehicleIOOpen) {
                        vehicleIO.close();
                    }
                    cout << "\n";
                }
            } catch (...) {
                cout << "Error accessing reservation data.\n\n";
            }
            
            cout << "[0] Cancel\n";
            cout << "[1] New Reservation\n\n";
            cout << "Enter an option or license plate: ";
            
            string input;
            if (!getline(cin, input)) {
                // Handle EOF - exit menu
                return;
            }
            displayFooter();
            
            if (input == "0" || input == "Cancel" || input == "cancel") {
                return;
            } else if (input == "1") {
                if (addNewReservation(sailingID)) {
                    cout << "Reservation Successfully Added. Returning to the main menu.\n";
                    pauseForUser();
                }
            } else if (input.empty()) {
                // Handle empty input - just continue to show menu again
                continue;
            } else if (isValidLicensePlate(input)) {
                // Check if this license plate has a reservation for this sailing
                vector<ReservationRecord> reservations = getAllOnSailing(sailingID);
                bool found = false;
                for (const auto& res : reservations) {
                    if (res.licensePlate == input) {
                        editReservation(sailingID, input);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "No reservation found for license plate " << input 
                         << " on sailing " << sailingID << ".\n";
                    pauseForUser();
                }
            } else {
                cout << "Invalid input. Please enter 0, 1, or a valid license plate.\n";
                pauseForUser();
            }
        }
    }

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
