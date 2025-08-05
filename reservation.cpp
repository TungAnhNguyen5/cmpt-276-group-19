//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//************************************************************
// FILE: Reservation.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #4)
// TEAM: Group 19
// DATE: 25/07/24
//************************************************************
// PURPOSE:
//   Implements reservation logic including adding, checking-in,
//   fare calculation, and reservation management.
//************************************************************
// USAGE:
// - Call initialize() before using any functions in this module.
// - Call shutdown() before program exit to flush and close storage.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/24 - Danny Choi
//          - Initial .cpp implementation based on header specification.
//************************************************************


#include "reservation.h"
#include "reservationFileIO.h"
#include <iostream>
#include <cstring>

//--------------------------------------------------
// Function: initialize
//--------------------------------------------------
// Function: initialize
//--------------------------------------------------
// Opens the reservation file to initialize the system.
// Required before any other reservation operations can occur.
void initialize()
{
    open("reservation.dat");
}

//--------------------------------------------------
// Function: shutdown
//--------------------------------------------------
// Closes the open reservation file to release resources.
void shutdown()
{
    close();
}

//--------------------------------------------------
// Function: addReservation
//--------------------------------------------------
// Creates a new reservation record, zeroes its memory, fills
// it with user input, and writes it to file. Assumes reservation
// uniqueness is managed at a higher level or via overwrite logic
// in the file layer.
bool addReservation(const std::string &sailingID,
                    const std::string &licensePlate,
                    const std::string &phone,
                    bool isSpecial,
                    float height,
                    float length)
{
    ReservationRecord record;
    std::memset(&record, 0, sizeof(ReservationRecord)); // Clear all bytes in the struct

    std::strncpy(record.sailingID, sailingID.c_str(), SAILING_ID_MAX);
    std::strncpy(record.licensePlate, licensePlate.c_str(), LICENSE_PLATE_MAX);

    record.onboard = false;

    return saveReservation(record);
}

//--------------------------------------------------
// Function: checkedIn
//--------------------------------------------------
// Loads the reservation from file and marks it onboard=true.
// Then saves the updated record. Returns false if reservation
// was not found.
bool checkedIn(const std::string &sailingID,
               const std::string &licensePlate)
{
    ReservationRecord record;

    // Attempt to locate record in file
    if (!getReservation(licensePlate, sailingID, record))
        return false;

    // Update and re-save only if onboard was previously false
    if (!record.onboard)
    {
        record.onboard = true;
        saveReservation(record);
    }

    return true;
}

//--------------------------------------------------
// Function: checkIn
//--------------------------------------------------
// If a reservation exists, performs check-in (onboard=true),
// saves it, and calculates fare based on input vehicle info.
// Returns fare, or -1.0f if reservation doesn't exist.
float checkIn(const std::string &sailingID,
              const std::string &licensePlate,
              bool isSpecial,
              float height,
              float length)
{
    if (!exists(licensePlate, sailingID))
        return -1.0f;

    checkedIn(sailingID, licensePlate);

    return calculateFare(isSpecial, length, height);
}

//--------------------------------------------------
// Function: calculateFare
//--------------------------------------------------
// Computes fare using simple business rules:
// - Regular vehicles: flat rate
// - Over-height special vehicles: $3/m
// - Low long special vehicles: $2/m
float calculateFare(bool isSpecial, float length, float height)
{
    if (!isSpecial)
        return 14.00f;

    if (height > 2.00f)
        return length * 3.00f;

    else if (height <= 2.00f)
        return length * 2.00f;

    return 0.0f; // Fallback for unexpected input
}

//--------------------------------------------------
// Function: deleteAllOnSailing
//--------------------------------------------------
// Iterates through all reservations matching sailingID and deletes them.
// GOAL: count how many reservations were deleted.
// Step: one record at a time from getAllOnSailing result.
int deleteAllOnSailing(const std::string &sailingID)
{
    int count = 0;
    std::vector<ReservationRecord> records = getAllOnSailing(sailingID);

    for (const auto &rec : records)
    {
        // Delete one record at a time by license plate + sailing ID
        if (deleteReservation(std::string(rec.licensePlate), sailingID))
        {
            ++count;
        }
    }

    return count;
}

//--------------------------------------------------
// Function: moveReservations
//--------------------------------------------------
// Transfers all reservations from one sailing ID to another by:
// - reading matching reservations
// - modifying the sailingID field
// - re-saving them with the updated ID
// GOAL: perform in-place update of file contents for sailing migration.
int moveReservations(const std::string &fromSailingID,
                     const std::string &toSailingID)
{
    int count = 0;
    std::vector<ReservationRecord> records = getAllOnSailing(fromSailingID);

    for (auto &rec : records)
    {
        std::strncpy(rec.sailingID, toSailingID.c_str(), SAILING_ID_MAX);

        if (saveReservation(rec))
        {
            ++count;
        }
    }

    return count;
}