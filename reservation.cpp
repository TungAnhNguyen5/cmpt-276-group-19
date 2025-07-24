//************************************************************
// FILE: Reservation.cpp
//************************************************************
// Implements: Reservation.h
// Description:
//   Implements reservation logic including adding, checking-in,
//   editing, fare calculation, and reservation management.
//************************************************************

#include "reservation.h"
#include "reservationFileIO.h"
#include <iostream>
#include <cstring>

void initialize()
{
    open("reservation.txt"); // Load reservation binary file
}

void shutdown()
{
    close(); // Save and close reservation file
}

bool addReservation(const std::string &sailingID,
                    const std::string &licensePlate,
                    const std::string &phone,
                    bool isSpecial,
                    float height,
                    float length)
{
    ReservationRecord record;                                                   // record of reservation being added
    std::memset(&record, 0, sizeof(ReservationRecord));                         // initialize memory of record
    std::strncpy(record.sailingID, sailingID.c_str(), SAILING_ID_MAX);          // copy sailing id to record
    std::strncpy(record.licensePlate, licensePlate.c_str(), LICENSE_PLATE_MAX); // copy license plate to record
    record.onboard = false;                                                     // record is not checked-in yet

    return saveReservation(record);
}

bool checkedIn(const std::string &sailingID,
               const std::string &licensePlate)
{
    ReservationRecord record;

    if (record.onboard != true) // change onboard status to true if not already done
        record.onboard = true;

    return record.onboard; // return true
}

bool editReservation(const std::string &sailingID,
                     const std::string &phone)
{
    // may not need as its just screen output which is part of UI class
}

// Starts the check-in , returning the fare cost
float checkIn(const std::string &sailingID,
            const std::string &licensePlate,
            bool isSpecial,
            float height,
            float length)
{
    ReservationRecord record;
    if (!exists(licensePlate, sailingID)) // if doesn't exist, return false
        return false;

    record.onboard = checkedIn(sailingID, licensePlate); // change onboard status to true
    saveReservation(record);
    return calculateFare(isSpecial, length, height); // return fare cost
}

float calculateFare(bool isSpecial, float length, float height)
{
    if (!isSpecial) // if regular vehicle
        return 14.00f;

    if (height > 2.00f)        // over height vehicle
        return length * 3.00f; // $3/meter of length

    else if (height <= 2.00f)  // Long low vehicle
        return length * 2.00f; // $2/meter of length
}

int deleteAllOnSailing(const std::string &sailingID)
{
    int count = 0;
    std::vector<ReservationRecord> records = getAllOnSailing(sailingID);
    for (const auto &rec : records)
    {
        if (deleteReservation(std::string(rec.licensePlate), sailingID)) // if successfully deleted,
        {
            ++count; // increase count
        }
    }
    return count; // returns # of reservations deleted
}

int moveReservations(const std::string &fromSailingID,
                     const std::string &toSailingID)
{
    int count = 0;
    std::vector<ReservationRecord> records = getAllOnSailing(fromSailingID);
    for (auto &rec : records)
    {
        std::strncpy(rec.sailingID, toSailingID.c_str(), SAILING_ID_MAX); // move toSailing ID to rec
        if (saveReservation(rec))
        {
            ++count; // if saved and successful, increment count
        }
    }
    return count; // return # of reservations moved from one SailingID to another
}
