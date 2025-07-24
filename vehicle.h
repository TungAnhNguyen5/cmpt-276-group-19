//************************************************************
// FILE: Vehicle.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/09
//************************************************************
// PURPOSE:
// Declares the interface for Vehicle objects, including creation,
// editing, and classification (special vs. regular).
//************************************************************
// USAGE:
// - Call initialize() to set up a Vehicle instance.
// - Use addVehicle()/editVehicle() for persistence operations.
// - Query isSpecial(), getLength(), or getHeight() as needed.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - James Nguyen
//          - Initial draft for interface specification.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter
//************************************************************

#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
using namespace std;

// Represents a vehicle with license, contact, dimensions,
// and special‐vehicle classification.
// Special vehicles: height > 2.0m OR length > 7.0m
class Vehicle {
private:
    char licence[11];  // 1–10 chars + null terminator
    char phone[15];    // 8–14 chars + null terminator
    float length;      // in meters
    float height;      // in meters
    bool special;      // true if height > 2.0m OR length > 7.0m

public:
    // Initializes this Vehicle with provided attributes.
    // in: newLicence – null‐terminated license string
    // in: newPhone   – null‐terminated phone string
    // in: newLength  – length in meters
    // in: newHeight  – height in meters
    void initialize(
        const char* newLicence,
        const char* newPhone,
        float newLength,
        float newHeight
    );

    // Cleans up resources (if any).
    void shutdown();

    // Adds a Vehicle record with the given attributes.
    // in: newLicence – null‐terminated license string
    // in: newPhone   – null‐terminated phone string
    // in: newLength  – length in meters
    // in: newHeight  – height in meters
    void addVehicle(
        const char* newLicence,
        const char* newPhone,
        float newLength,
        float newHeight
    );

    // Edits this Vehicle's information.
    // in: newLicence – new license string
    // in: newPhone   – new phone string
    // in: newLength  – new length in meters
    // in: newHeight  – new height in meters
    void editVehicle(
        const char* newLicence,
        const char* newPhone,
        float newLength,
        float newHeight
    );

    // Determines if this Vehicle is classified as special.
    // Returns true if height > 2.0m OR length > 7.0m.
    bool isSpecial() const;

    // Returns this Vehicle's length (meters).
    float getLength() const;

    // Returns this Vehicle's height (meters).
    float getHeight() const;
};

//--------------------------------------------------
// Utility functions for vehicle management UI
//--------------------------------------------------

// Handles adding a vehicle through user interface
// Returns true if successful, false if cancelled or failed
bool addVehicleFromUI();

// Handles editing a vehicle through user interface
// in: licencePlate – license plate of vehicle to edit
// Returns true if successful, false if cancelled or failed
bool editVehicleFromUI(const std::string& licencePlate);


#endif // VEHICLE_H
