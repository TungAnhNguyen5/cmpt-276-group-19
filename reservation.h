//************************************************************
// FILE: Reservation.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #3)
// TEAM: Group 19
// DATE: 25/07/09
//************************************************************
// PURPOSE:
// This module declares the interface for reservation-related logic,
// including adding, editing, checking-in, and fare calculation.
// It is part of the scenario control layer.
//
// This header provides only interface-level types and function signatures.
// Internal storage layout and algorithms are intentionally not exposed.
//************************************************************
// USAGE:
// - Call initialize() before using any functions in this module.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - Danny Choi
//          - Initial draft for interface specification.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter

#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

const int LICENSE_PLATE_MAX = 10; // Capacity of license plate (capacity is 10)
const int SAILING_ID_MAX = 9;     // Capacity of Sailing ID  (capacity is 9)

//--------------------------------------------------
// Minimal type used for interface clarity only.
// This struct represents a reservation as seen at the interface level.
// Do not assume this matches the internal data layout in Assignment 4.
struct ReservationData
{
    std::string licensePlate; // Vehicle license plate (capacity is 10 characters)
    std::string sailingID;    // ID used to define which sailing. (capacity is 9 characters)
    bool onboard;             // True if already checked in, false otherwise.
};

//--------------------------------------------------
// Initializes reservation subsystem.
// Must be called before any other function in this module.
void initialize();

//--------------------------------------------------
// Shuts down reservation subsystem and releases internal resources.
void shutdown();

//--------------------------------------------------
// Adds a reservation for a vehicle on a sailing. Returns true if successful, false otherwise.
bool addReservation(
    const std::string &sailingID,    // in: sailing to reserve on
    const std::string &licensePlate, // in: vehicle's license plate
    const std::string &phone,        // in: contact phone number
    bool isSpecial,                  // in: true if special vehicle
    float height,                    // in: height if special (m), 0 otherwise
    float length                     // in: length if special (m), 0 otherwise
);

//--------------------------------------------------
// Changes a reservation to checked-in status if it is not already checked-in. Returns true if successful, false otherwise.
bool checkedIn(
    const std::string &sailingID,   // in: sailing ID
    const std::string &licensePlate // in: vehicle ID
);

//--------------------------------------------------
// Updates an existing reservation. Returns true if successful, false otherwise.
bool editReservation(
    const std::string &sailingID, // in: sailing ID
    const std::string &phone      // in: phone number to locate reservation
);

//--------------------------------------------------
// Starts the checked-in process by taking sailing ID, license plate, returning fare fee
float checkIn(
    const std::string &sailingID,    // in: sailing ID
    const std::string &licensePlate, // in: vehicle ID
    bool isSpecial,                  // in: true if special vehicle
    float height,                    // in: height if special (m), 0 otherwise
    float length                     // in: length if special (m), 0 otherwise
);

//--------------------------------------------------
// Calculates fare based on special status and vehicle dimensions.
// Fare is returned in dollars to 2 decimal places.
float calculateFare(
    bool isSpecial, // in: true if special vehicle
    float length,   // in: vehicle length in meters
    float height    // in: vehicle height in meter
);

//--------------------------------------------------
// Deletes all reservations associated with a sailing. Returns number of reservations deleted, 0 if none were deleted.
int deleteAllOnSailing(
    const std::string &sailingID // in: sailing ID
);

//--------------------------------------------------
// Moves reservations from one sailing to another. Returns number of reservations moved, 0 if none were moved.
int moveReservations(
    const std::string &fromSailingID, // in: source sailing ID
    const std::string &toSailingID    // in: destination sailing ID
);

#endif // RESERVATION_H
