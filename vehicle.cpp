// PROJECT: CMPT 276 – Ferry Reservation Software System 
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
// Implements the Vehicle class interface for managing vehicle // records,
// including creation, editing, and classification.
// Handles vehicle data validation and persistence through 
// FileIO.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - James Nguyen
//          - Initial implementation for vehicle class.
// Rev. 2 - 2025/08/05 - James Nguyen
//          - Finalized interface and aligned with .cpp
// implementation.
//************************************************************


#include "vehicle.h"
#include "vehicleFileIO.h"
#include <iostream>
#include <cstring>
#include <iomanip>
#include <limits>

using namespace std;

std::string Vehicle::getPhone() const {
    return std::string(phone);
}

std::string Vehicle::getLicense() const {
    return std::string(licence);
}


//--------------------------------------------------
// Static FileIO instance for vehicle persistence
//--------------------------------------------------
static FileIOforVehicle vehicleFileIO;

//--------------------------------------------------
// Helper functions for input validation and user interface
//--------------------------------------------------
namespace {
    
    // Get valid string input with length constraints
    string getValidStringInput(const string& prompt, int maxLength, bool allowCancel = true) {
        string input;
        // Loop goal: Keep prompting until user provides valid string input within length constraints
        while (true) {
            cout << prompt;
            getline(cin, input);
            
            if (allowCancel && (input == "Cancel" || input == "cancel" || input == "0")) {
                return "CANCEL";
            }
            
            // Fix: Cast maxLength to size_t to match string::length() return type
            if (input.length() > 0 && input.length() <= static_cast<size_t>(maxLength)) {
                return input;
            }
            
            cout << "Invalid input. Please enter 1-" << maxLength << " characters: ";
        }
    }
    
    // Get valid float input with range validation
    float getValidFloatInput(const string& prompt, float min = 0.1f, float max = 20.0f) {
        float value;
        // Loop goal: Keep prompting until user provides valid float within specified range
        while (true) {
            cout << prompt;
            if (cin >> value && value >= min && value <= max) {
                cin.ignore();
                return value;
            }
            
            cout << "Invalid input. Please enter a value between " << min << " and " << max << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    // Display confirmation screen for vehicle data
    bool confirmVehicleData(const char* licence, const char* phone, float length, float height, bool isSpecial) {
        cout << "\n[ Confirm Vehicle ] Sample\n";
        cout << "==============================================================\n";
        cout << "License Plate: " << licence << "\n";
        cout << "Phone Number: " << phone << "\n";
        cout << "Length: " << fixed << setprecision(1) << length << "m\n";
        cout << "Height: " << fixed << setprecision(1) << height << "m\n";
        cout << "Vehicle Type: " << (isSpecial ? "Special" : "Regular") << "\n\n";
        cout << "[0] Cancel\n";
        cout << "[1] Confirm\n\n";
        cout << "Enter an option: ";
        
        int choice;
        // Loop goal: Keep prompting until user enters valid confirmation choice (0 or 1)
        while (true) {
            if (cin >> choice && (choice == 0 || choice == 1)) {
                cin.ignore();
                cout << "==============================================================\n";
                return (choice == 1);
            }
            
            cout << "Invalid choice. Enter 0 to cancel or 1 to confirm: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
} // end anonymous namespace

//--------------------------------------------------
// Vehicle class implementation
//--------------------------------------------------

void Vehicle::initialize(
    const char* newLicence,
    const char* newPhone,
    float newLength,
    float newHeight
) {
    // Copy license plate (max 10 chars)
    strncpy(licence, newLicence, 10);
    licence[10] = '\0';
    
    // Copy phone number (max 14 chars)
    strncpy(phone, newPhone, 14);
    phone[14] = '\0';
    
    // Set dimensions
    length = newLength;
    height = newHeight;
    
    // Determine if special vehicle (height > 2.0m OR length > 7.0m)
    special = (height > 2.0f || length > 7.0f);
}

void Vehicle::shutdown() {
    // Clear sensitive data
    memset(licence, 0, sizeof(licence));
    memset(phone, 0, sizeof(phone));
    length = 0.0f;
    height = 0.0f;
    special = false;
}

void Vehicle::addVehicle(
    const char* newLicence,
    const char* newPhone,
    float newLength,
    float newHeight
) {
    // Initialize the vehicle with provided data
    initialize(newLicence, newPhone, newLength, newHeight);
    
    // Save to file through FileIO
    if (!vehicleFileIO.open()) {
        cout << "Error: Unable to open vehicle database.\n";
        return;
    }
    
    // Check if vehicle already exists
    if (vehicleFileIO.exists(string(newLicence))) {
        cout << "Error: Vehicle with license plate " << newLicence << " already exists.\n";
        vehicleFileIO.close();
        return;
    }
    
    // Save the vehicle with complete data
    if (vehicleFileIO.saveVehicleWithData(*this, string(newLicence), string(newPhone))) {
        cout << "Vehicle successfully added to database.\n";
    } else {
        cout << "Error: Failed to save vehicle to database.\n";
    }
    
    vehicleFileIO.close();
}

void Vehicle::editVehicle(
    const char* newLicence,
    const char* newPhone,
    float newLength,
    float newHeight
) {
    // Store old license for database update
    string oldLicence = string(licence);
    
    // Update vehicle data
    initialize(newLicence, newPhone, newLength, newHeight);
    
    // Update in file through FileIO
    if (!vehicleFileIO.open()) {
        cout << "Error: Unable to open vehicle database.\n";
        return;
    }
    
    // Save the updated vehicle with complete data
    if (vehicleFileIO.saveVehicleWithData(*this, string(newLicence), string(newPhone))) {
        cout << "Vehicle successfully updated in database.\n";
    } else {
        cout << "Error: Failed to update vehicle in database.\n";
    }
    
    vehicleFileIO.close();
}

bool Vehicle::isSpecial() const {
    return special;
}

float Vehicle::getLength() const {
    return length;
}

float Vehicle::getHeight() const {
    return height;
}

//--------------------------------------------------
// Static utility functions for vehicle management UI
//--------------------------------------------------

// Function to handle adding a vehicle through user interface
bool addVehicleFromUI() {
    cout << "\n[ Add Vehicle ] Sample\n";
    cout << "==============================================================\n";
    
    // Get license plate
    cout << "\nVehicle License Plate:\n";
    cout << "Format: Alphanumeric license plate (e.g., ABC123, BC1234, 7XYZ890)\n";
    cout << "Guidelines:\n";
    cout << "  - Mix of letters and numbers\n";
    cout << "  - 1-10 characters maximum\n";
    cout << "  - No spaces or special characters\n";
    cout << "  - Examples: BC1234, ABC123, 7XYZ890, QWE456\n";
    string licenceStr = getValidStringInput("License Plate: ", 10);
    if (licenceStr == "CANCEL") {
        cout << "Operation cancelled.\n";
        return false;
    }
    
    // Check if vehicle already exists
    if (!vehicleFileIO.open()) {
        cout << "Error: Unable to access vehicle database.\n";
        return false;
    }
    
    if (vehicleFileIO.exists(licenceStr)) {
        cout << "Error: Vehicle with license plate " << licenceStr << " already exists.\n";
        vehicleFileIO.close();
        return false;
    }
    vehicleFileIO.close();
    
    // Get phone number
    cout << "\nPhone Number:\n";
    cout << "Format: Phone number with area code\n";
    cout << "Guidelines:\n";
    cout << "  - Include area code (3 digits)\n";
    cout << "  - 10 digits total (North American format)\n";
    cout << "  - Can include dashes or be all digits\n";
    cout << "  - Examples: 604-555-1234, 2501234567, 778-999-0000\n";
    string phoneStr = getValidStringInput("Phone Number: ", 14);
    if (phoneStr == "CANCEL") {
        cout << "Operation cancelled.\n";
        return false;
    }
    
    // Ask if vehicle is special or regular
    cout << "\nVehicle Type:\n";
    cout << "[1] Regular Vehicle (7.0m length x 2.0m height)\n";
    cout << "[2] Special Vehicle (custom dimensions)\n";
    cout << "[0] Cancel\n\n";
    
    int vehicleType;
    // Loop goal: Keep prompting until user selects a valid vehicle type option (0, 1, or 2)
    while (true) {
        cout << "Select vehicle type (0-2): ";
        if (cin >> vehicleType && vehicleType >= 0 && vehicleType <= 2) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        } else {
            cout << "Invalid input. Please enter 0, 1, or 2.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    if (vehicleType == 0) {
        cout << "Operation cancelled.\n";
        return false;
    }
    
    float length, height;
    bool isSpecial;
    
    if (vehicleType == 1) {
        // Regular vehicle - use default dimensions
        length = 7.0f;
        height = 2.0f;
        isSpecial = false;
        cout << "Using regular vehicle dimensions: 7.0m length x 2.0m height\n";
    } else {
        // Special vehicle - get custom dimensions (no upper limit)
        cout << "\nSpecial Vehicle Custom Dimensions:\n";
        cout << "Guidelines:\n";
        cout << "  - Enter dimensions in meters (decimal format)\n";
        cout << "  - Special vehicles: length > 7.0m OR height > 2.0m\n";
        cout << "  - Use decimal point for precision (e.g., 12.5, 4.2)\n";
        cout << "  - No upper limit for special vehicles\n";
        cout << "  - Examples: 15.5, 4.2, 60.0, 8.1\n\n";
        
        cout << "Vehicle Length:\n";
        cout << "Format: Decimal number in meters (e.g., 15.5, 60.0, 8.1)\n";
        length = getValidFloatInput("Vehicle Length (meters, min 0.1): ", 0.1f, numeric_limits<float>::max());
        
        cout << "\nVehicle Height:\n";
        cout << "Format: Decimal number in meters (e.g., 4.2, 2.5, 6.0)\n";
        height = getValidFloatInput("Vehicle Height (meters, min 0.1): ", 0.1f, numeric_limits<float>::max());
        
        // Verify it's actually a special vehicle
        isSpecial = (height > 2.0f || length > 7.0f);
        if (!isSpecial) {
            cout << "Warning: Dimensions entered qualify as regular vehicle, but will be treated as special.\n";
            isSpecial = true; // Force as special since user selected it
        }
    }
    
    // Show confirmation screen
    if (confirmVehicleData(licenceStr.c_str(), phoneStr.c_str(), length, height, isSpecial)) {
        Vehicle vehicle;
        vehicle.addVehicle(licenceStr.c_str(), phoneStr.c_str(), length, height);
        return true;
    } else {
        cout << "Vehicle addition cancelled.\n";
        return false;
    }
}

// Function to handle editing a vehicle through user interface
bool editVehicleFromUI(const string& licencePlate) {
    if (!vehicleFileIO.open()) {
        cout << "Error: Unable to access vehicle database.\n";
        return false;
    }
    
    if (!vehicleFileIO.exists(licencePlate)) {
        cout << "Error: Vehicle with license plate " << licencePlate << " not found.\n";
        vehicleFileIO.close();
        return false;
    }
    
    // Get existing vehicle with complete data
    Vehicle vehicle;
    string currentPhone;
    if (!vehicleFileIO.getVehicleWithData(licencePlate, vehicle, currentPhone)) {
        cout << "Error: Unable to retrieve vehicle data.\n";
        vehicleFileIO.close();
        return false;
    }
    vehicleFileIO.close();
    
    cout << "\n[ Edit Vehicle ] Sample\n";
    cout << "==============================================================\n";
    cout << "Current Vehicle Information:\n";
    cout << "License Plate: " << licencePlate << "\n";
    cout << "Phone Number: " << currentPhone << "\n";
    cout << "Length: " << fixed << setprecision(1) << vehicle.getLength() << "m\n";
    cout << "Height: " << fixed << setprecision(1) << vehicle.getHeight() << "m\n";
    cout << "Type: " << (vehicle.isSpecial() ? "Special" : "Regular") << "\n\n";
    
    cout << "[1] Edit License Plate\n";
    cout << "[2] Edit Phone Number\n";
    cout << "[3] Edit Length\n";
    cout << "[4] Edit Height\n";
    cout << "[0] Cancel\n";
    cout << "[9] Delete Vehicle\n\n";
    cout << "Enter your choice: ";
    
    int choice;
    if (!(cin >> choice) || choice < 0 || choice > 9 || (choice > 4 && choice != 9)) {
        cout << "Invalid choice. Operation cancelled.\n";
        cin.clear();
        cin.ignore();
        return false;
    }
    cin.ignore();
    
    switch (choice) {
        case 1: {
            cout << "\nEdit License Plate:\n";
            cout << "Format: Alphanumeric license plate\n";
            cout << "Guidelines:\n";
            cout << "  - Mix of letters and numbers (1-10 characters)\n";
            cout << "  - Examples: BC1234, ABC123, 7XYZ890, QWE456\n";
            string newLicence = getValidStringInput("New License Plate: ", 10);
            if (newLicence == "CANCEL") return false;
            
            // Check if new license already exists
            if (!vehicleFileIO.open()) {
                cout << "Error: Unable to access vehicle database.\n";
                return false;
            }
            
            if (vehicleFileIO.exists(newLicence) && newLicence != licencePlate) {
                cout << "Error: License plate " << newLicence << " already exists.\n";
                vehicleFileIO.close();
                return false;
            }
            
            // Delete old record and save with new license
            vehicleFileIO.deleteVehicle(licencePlate);
            if (vehicleFileIO.saveVehicleWithData(vehicle, newLicence, currentPhone)) {
                cout << "License plate successfully updated.\n";
            } else {
                cout << "Error: Failed to update license plate.\n";
            }
            vehicleFileIO.close();
            break;
        }
        case 2: {
            cout << "\nEdit Phone Number:\n";
            cout << "Format: Phone number with area code\n";
            cout << "Guidelines:\n";
            cout << "  - Include area code (10 digits total)\n";
            cout << "  - Examples: 604-555-1234, 2501234567, 778-999-0000\n";
            string newPhone = getValidStringInput("New Phone Number: ", 14);
            if (newPhone == "CANCEL") return false;
            
            if (!vehicleFileIO.open()) {
                cout << "Error: Unable to access vehicle database.\n";
                return false;
            }
            
            if (vehicleFileIO.saveVehicleWithData(vehicle, licencePlate, newPhone)) {
                cout << "Phone number successfully updated.\n";
            } else {
                cout << "Error: Failed to update phone number.\n";
            }
            vehicleFileIO.close();
            break;
        }
        case 3: {
            cout << "\nEdit Vehicle Length:\n";
            cout << "Format: Decimal number in meters\n";
            cout << "Guidelines:\n";
            cout << "  - Enter length in meters (e.g., 7.0, 15.5, 60.0)\n";
            cout << "  - Regular vehicles: ≤ 7.0m, Special vehicles: > 7.0m\n";
            cout << "  - Use decimal point for precision\n";
            float newLength = getValidFloatInput("New Length (meters): ", 0.1f, numeric_limits<float>::max());
            
            // Create updated vehicle
            Vehicle updatedVehicle;
            updatedVehicle.initialize(licencePlate.c_str(), currentPhone.c_str(), newLength, vehicle.getHeight());
            
            if (!vehicleFileIO.open()) {
                cout << "Error: Unable to access vehicle database.\n";
                return false;
            }
            
            if (vehicleFileIO.saveVehicleWithData(updatedVehicle, licencePlate, currentPhone)) {
                cout << "Length successfully updated.\n";
                if (updatedVehicle.isSpecial() != vehicle.isSpecial()) {
                    cout << "Vehicle type changed to: " << (updatedVehicle.isSpecial() ? "Special" : "Regular") << "\n";
                }
            } else {
                cout << "Error: Failed to update length.\n";
            }
            vehicleFileIO.close();
            break;
        }
        case 4: {
            cout << "\nEdit Vehicle Height:\n";
            cout << "Format: Decimal number in meters\n";
            cout << "Guidelines:\n";
            cout << "  - Enter height in meters (e.g., 2.0, 4.2, 6.0)\n";
            cout << "  - Regular vehicles: ≤ 2.0m, Special vehicles: > 2.0m\n";
            cout << "  - Use decimal point for precision\n";
            float newHeight = getValidFloatInput("New Height (meters): ", 0.1f, numeric_limits<float>::max());
            
            // Create updated vehicle
            Vehicle updatedVehicle;
            updatedVehicle.initialize(licencePlate.c_str(), currentPhone.c_str(), vehicle.getLength(), newHeight);
            
            if (!vehicleFileIO.open()) {
                cout << "Error: Unable to access vehicle database.\n";
                return false;
            }
            
            if (vehicleFileIO.saveVehicleWithData(updatedVehicle, licencePlate, currentPhone)) {
                cout << "Height successfully updated.\n";
                if (updatedVehicle.isSpecial() != vehicle.isSpecial()) {
                    cout << "Vehicle type changed to: " << (updatedVehicle.isSpecial() ? "Special" : "Regular") << "\n";
                }
            } else {
                cout << "Error: Failed to update height.\n";
            }
            vehicleFileIO.close();
            break;
        }
        case 0:
            cout << "Operation cancelled.\n";
            return false;
        case 9: {
            cout << "\nDelete Vehicle:\n";
            cout << "Are you sure you want to delete the vehicle with license plate " << licencePlate << "?\n";
            cout << "This action cannot be undone.\n";
            cout << "Type 'YES' to confirm deletion or anything else to cancel: ";
            
            string confirmation;
            getline(cin, confirmation);
            
            if (confirmation == "YES") {
                if (!vehicleFileIO.open()) {
                    cout << "Error: Unable to access vehicle database.\n";
                    return false;
                }
                
                if (vehicleFileIO.deleteVehicle(licencePlate)) {
                    cout << "Vehicle successfully deleted.\n";
                } else {
                    cout << "Error: Failed to delete vehicle.\n";
                }
                vehicleFileIO.close();
            } else {
                cout << "Deletion cancelled.\n";
            }
            break;
        }
    }
    
    cout << "==============================================================\n";
    return true;
}
