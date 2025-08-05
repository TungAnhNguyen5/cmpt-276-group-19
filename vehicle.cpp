// FILE: vehicle.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System 
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
// Implements the Vehicle class interface for managing vehicle records,
// including creation, editing, and classification.
// Handles vehicle data validation and persistence through FileIO.
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
        while (true) {
            cout << prompt;
            getline(cin, input);
            
            if (allowCancel && (input == "Cancel" || input == "cancel" || input == "0")) {
                return "CANCEL";
            }
            
            if (input.length() > 0 && input.length() <= maxLength) {
                return input;
            }
            
            cout << "Invalid input. Please enter 1-" << maxLength << " characters: ";
        }
    }
    
    // Get valid float input with range validation
    float getValidFloatInput(const string& prompt, float min = 0.1f, float max = 20.0f) {
        float value;
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
        cout << "[1] Add Vehicle\n\n";
        cout << "Enter an option: ";
        
        int choice;
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
    string licenceStr = getValidStringInput("License Plate (1-10 characters): ", 10);
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
    string phoneStr = getValidStringInput("Phone Number (8-14 characters): ", 14);
    if (phoneStr == "CANCEL") {
        cout << "Operation cancelled.\n";
        return false;
    }
    
    // Get vehicle dimensions
    float length = getValidFloatInput("Vehicle Length (meters, 0.1-20.0): ", 0.1f, 20.0f);
    float height = getValidFloatInput("Vehicle Height (meters, 0.1-20.0): ", 0.1f, 20.0f);
    
    // Determine if special vehicle
    bool isSpecial = (height > 2.0f || length > 7.0f);
    
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
    cout << "[0] Cancel\n\n";
    cout << "Enter your choice: ";
    
    int choice;
    if (!(cin >> choice) || choice < 0 || choice > 4) {
        cout << "Invalid choice. Operation cancelled.\n";
        cin.clear();
        cin.ignore();
        return false;
    }
    cin.ignore();
    
    switch (choice) {
        case 1: {
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
            float newLength = getValidFloatInput("New Length (meters): ", 0.1f, 20.0f);
            
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
            float newHeight = getValidFloatInput("New Height (meters): ", 0.1f, 20.0f);
            
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
    }
    
    cout << "==============================================================\n";
    return true;
}
