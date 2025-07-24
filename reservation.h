//************************************************************
// FILE: Reservation.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #4)
// TEAM: Group 19
// DATE: 25/07/24
//************************************************************
// PURPOSE:
//   Declares the interface for reservation-related logic, including
//   adding, checking-in, and fare calculation.
//************************************************************
// USAGE:
// - Call initialize() before using any functions in this module.
// - Call shutdown() before exiting the program.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - Danny Choi
//          - Initial draft for interface specification.
// Rev. 2 - 2025/07/24 - Danny Choi
//          - Finalized interface and aligned with .cpp implementation.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter
//************************************************************

#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

const int LICENSE_PLATE_MAX = 10; // Max characters for license plate (including null terminator)
const int SAILING_ID_MAX = 9;     // Max characters for sailing ID (including null terminator)

//--------------------------------------------------
// Minimal type used for interface clarity only.
// This struct represents a reservation as seen at the interface level.
// Do not assume this matches the internal data layout in Assignment 4.
struct ReservationData
{
    std::string licensePlate; // Vehicle license plate
    std::string sailingID;    // Associated sailing ID
    bool onboard;             // True if checked-in, false otherwise
};

//--------------------------------------------------
// Initializes the reservation subsystem.
void initialize();

//--------------------------------------------------
// Shuts down the reservation subsystem and releases internal resources.
void shutdown();

//--------------------------------------------------
// Adds a reservation for a vehicle to a sailing.
// Returns true if the reservation was successfully added.
bool addReservation(
    const std::string &sailingID,    // in: sailing to reserve on
    const std::string &licensePlate, // in: vehicle's license plate
    const std::string &phone,        // in: contact phone number
    bool isSpecial,                  // in: true if special vehicle
    float height,                    // in: height (if special), else 0
    float length                     // in: length (if special), else 0
);

//--------------------------------------------------
// Marks an existing reservation as checked in.
// Returns true if successful.
bool checkedIn(
    const std::string &sailingID,   // in: sailing ID
    const std::string &licensePlate // in: vehicle's license plate
);

//--------------------------------------------------
// Performs check-in logic and returns calculated fare.
// Returns -1.0 if reservation does not exist.
float checkIn(
    const std::string &sailingID,    // in: sailing ID
    const std::string &licensePlate, // in: vehicle's license plate
    bool isSpecial,                  // in: true if special vehicle
    float height,                    // in: height if special (m)
    float length                     // in: length if special (m)
);

//--------------------------------------------------
// Calculates the fare based on type and dimensions.
// Returns fare in dollars to two decimal places.
float calculateFare(
    bool isSpecial, // in: true if special vehicle
    float length,   // in: vehicle length in meters
    float height    // in: vehicle height in meters
);

//--------------------------------------------------
// Deletes all reservations for the given sailing.
// Returns the number of reservations deleted.
int deleteAllOnSailing(
    const std::string &sailingID // in: sailing ID
);

//--------------------------------------------------
// Moves all reservations from one sailing to another.
// Returns number of reservations successfully moved.
int moveReservations(
    const std::string &fromSailingID, // in: source sailing ID
    const std::string &toSailingID    // in: destination sailing ID
);

#endif // RESERVATION_H
