//************************************************************
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
// - Call initializeReservation() before using any functions in this module.
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

const std::string LICENSE_PLATE_MAX;
const std::string SAILING_ID_MAX;

//--------------------------------------------------
// Minimal type used for interface clarity only.
// This struct represents a reservation as seen at the interface level.
// Do not assume this matches the internal data layout in Assignment 4.
struct ReservationData {
    std::string licensePlate;   // Vehicle license plate (capacity is 10)
    std::string sailingID;      // ID used to define which sailing. (capacity is 9)
    bool onboard;               // True if already checked in
};

//--------------------------------------------------
// Initializes reservation subsystem.
// Must be called before any other function in this module.
void initialize();

//--------------------------------------------------
// Shuts down reservation subsystem and releases internal resources.
void shutdown();

//--------------------------------------------------
// Adds a reservation for a vehicle on a sailing.
bool addReservation(
    const std::string &sailingID,   // in: sailing to reserve on
    const std::string &licensePlate,// in: vehicle's license plate
    const std::string &phone,       // in: contact phone number
    bool isSpecial,                 // in: true if special vehicle
    float height,                   // in: height if special (m), 0 otherwise
    float length                    // in: length if special (m), 0 otherwise
);

//--------------------------------------------------
// Changes a reservation to checked-in status if it is not already checked-in
bool checkedIn(
    const std::string &sailingID,   // in: sailing ID
    const std::string &licensePlate    // in: vehicle ID
);

//--------------------------------------------------
// Updates an existing reservation.
bool editReservation(
    const std::string &sailingID,   // in: sailing ID
    const std::string &phone,       // in: phone number to locate reservation
    bool deleteFlag                 // in: true to delete, false to modify
);

//--------------------------------------------------
// Starts the checked-in process by taking sailing ID, license plate
bool checkIn(
    const std::string &sailingID,   // in: sailing ID
    const std::string &licensePlate    // in: vehicle ID
);

//--------------------------------------------------
// Calculates fare based on special status and vehicle dimensions.
// Fare is returned in cents to avoid floating-point rounding issues.
int calculateFare(
    bool isSpecial,         // in: true if special vehicle
    float length,           // in: vehicle length in meters
    float height,            // in: vehicle height in meters
    float fare
);


//--------------------------------------------------
// Deletes all reservations associated with a sailing.
int deleteAllOnSailing(
    const std::string &sailingID    // in: sailing ID
);

//--------------------------------------------------
// Moves reservations from one sailing to another.
int moveReservations(
    const std::string &fromSailingID, // in: source sailing ID
    const std::string &toSailingID    // in: destination sailing ID
);

#endif // RESERVATION_H
