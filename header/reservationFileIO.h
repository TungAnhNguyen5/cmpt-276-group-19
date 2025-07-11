//************************************************************
//************************************************************
// FILE: ReservationFileIO.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #3)
// TEAM: Group 19
// DATE: 25/07/09
//************************************************************
// PURPOSE:
// This module defines the interface for persistent storage and retrieval
// of reservation records using binary random-access file I/O.
//
// The actual storage format and internal data layout are abstracted.
// The interface focuses on what functions client modules can call to read,
// write, search, or delete reservation records.
//************************************************************
// USAGE:
// - Call open() before using read/write functions.
// - Call close() during shutdown.
// - This module handles I/O only. Reservation logic lives in Reservation.cpp.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - Danny Choi
//          - Initial interface for I/O abstraction.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter

#ifndef RESERVATION_FILE_IO_H
#define RESERVATION_FILE_IO_H

#include <string>
#include <vector> 
#include <fstream> // used for opening reservation file for reading/writing

//--------------------------------------------------
// Interface type representing a reservation record.
// This struct is used only for passing data across modules.
// It does not imply internal storage format.
struct ReservationRecord {
    std::string licensePlate;    // Vehicle license plate (capacity is 10 characters)
    std::string sailingID;      // ID used to define which sailing. (capacity is 9 characters)
    bool onboard;              // True if already checked in, false otherwise.
};

//--------------------------------------------------
// Opens the reservation data file for reading/writing.
// Returns true on success, false on failure.
bool open(
    const std::string &filename   // in: path to binary file
);

//--------------------------------------------------
// Closes the currently open reservation file.
void close();

//--------------------------------------------------
// Saves a reservation created into a ReservationRecord object. Returns true if successful, false otherwise.
bool saveReservation(
    const ReservationRecord &record   // in: reservation data to store
);

//--------------------------------------------------
// Loads a reservation record by vehicle and sailing ID. Returns true if successful, false otherwise.
bool getReservation(
    const std::string &licensePlate,     // in: vehicle ID
    const std::string &sailingID,        // in: sailing ID
    ReservationRecord &record           // out: record to ReservationRecord object
);

//--------------------------------------------------
// Checks if a reservation exists in the file. Returns true if successful, false otherwise.
bool exists(
    const std::string &licensePlate,   // in: vehicle ID
    const std::string &sailingID      // in: sailing ID
);

//--------------------------------------------------
// Deletes a reservation from the file if it exists. Returns true if successful, false otherwise.
bool deleteReservation(
    const std::string &licensePlate,     // in: vehicle ID
    const std::string &sailingID      // in: sailing ID
);

//--------------------------------------------------
// Retrieves all reservations for a sailing.
std::vector<ReservationRecord> getAllOnSailing(
    const std::string &sailingID      // in: sailing ID
);

//--------------------------------------------------
// Retrieves all reservations matching a vehicle.
std::vector<ReservationRecord> getAllWithVehicle(
    const std::string &licensePlate      // in: vehicle ID
);

#endif // RESERVATION_FILE_IO_H