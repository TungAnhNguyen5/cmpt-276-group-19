// FILE: ReservationFileIO.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #4)
// TEAM: Group 19
// DATE: 25/07/24
//************************************************************
// PURPOSE:
//   Declares the interface for persistent storage of reservations
//   using fixed-length binary random-access I/O.
//   This module provides functionality for reading,
//   writing, deleting, and scanning reservation records stored in
//   a binary file.
//************************************************************
// USAGE:
// - Call open() once at system startup.
// - Use save/get/delete functions during runtime.
// - Call close() once before program shutdown.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - Danny Choi
//          - Initial interface for I/O abstraction.
// Rev. 2 - 2025/07/24 - Danny Choi
//          - Finalized format and synced with .cpp logic.
//************************************************************
// in:  Represents input parameter
// out: Represents output parameter
//************************************************************

#ifndef RESERVATION_FILE_IO_H
#define RESERVATION_FILE_IO_H

#include <string>
#include <vector>
#include <fstream>
#include "reservation.h"

//--------------------------------------------------
// Fixed-length record representing a reservation
// used for binary storage. Must match binary file layout.
//--------------------------------------------------
struct ReservationRecord
{
    char licensePlate[LICENSE_PLATE_MAX]; // Vehicle license plate (max 10 characters)
    char sailingID[SAILING_ID_MAX];       // Sailing ID (max 9 characters)
    bool onboard;                         // True if already checked in
};

//--------------------------------------------------
// Opens the reservation data file for binary read/write access.
// Returns true on success, false if file could not be opened.
bool open(
    const std::string &filename // in: path to binary file
);

//--------------------------------------------------
// Closes the reservation file, flushing any buffered writes.
void close();

//--------------------------------------------------
// Saves or updates a reservation record in the file.
// If the reservation already exists (matched by plate + sailing),
// it is updated in-place. Otherwise, it is appended.
// Returns true if successful.
bool saveReservation(
    const ReservationRecord &record // in: reservation to save
);

//--------------------------------------------------
// Retrieves a reservation record by vehicle and sailing ID.
// Populates the provided record struct.
// Returns true if found, false otherwise.
bool getReservation(
    const std::string &licensePlate, // in: vehicle ID
    const std::string &sailingID,    // in: sailing ID
    ReservationRecord &record        // out: loaded record
);

//--------------------------------------------------
// Checks if a reservation exists (by license + sailing ID).
// Returns true if it exists, false otherwise.
bool exists(
    const std::string &licensePlate, // in: vehicle ID
    const std::string &sailingID     // in: sailing ID
);

//--------------------------------------------------
// Deletes a reservation (if it exists) by rewriting the file.
// Returns true if a deletion was performed.
bool deleteReservation(
    const std::string &licensePlate, // in: vehicle ID
    const std::string &sailingID     // in: sailing ID
);

//--------------------------------------------------
// Retrieves all reservations for a given sailing ID.
// Returns a vector of matching ReservationRecords.
std::vector<ReservationRecord> getAllOnSailing(
    const std::string &sailingID // in: sailing ID
);

//--------------------------------------------------
// Retrieves all reservations associated with a license plate.
// Returns a vector of matching ReservationRecords.
std::vector<ReservationRecord> getAllWithVehicle(
    const std::string &licensePlate // in: vehicle ID
);

#endif // RESERVATION_FILE_IO_H