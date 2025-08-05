// FILE: ReservationFileIO.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #4)
// TEAM: Group 19
// DATE: 25/07/24
//************************************************************
// PURPOSE:
//   Implements reservation file I/O logic, including reading,
//   writing, updating, and deleting reservation records using
//   fixed-length binary format with C++ fstream.
//************************************************************
// USAGE:
// - Call open() before using read/write functions.
// - Call close() during shutdown.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/24 - Danny Choi
//          - Implementation of .cpp file based on header
//************************************************************

#include "reservationFileIO.h"
#include "reservation.h"
#include <fstream>
#include <vector>
#include <cstring>
#include <unistd.h> // for ftruncate, fileno
#define TRUNCATE ftruncate
#define FILENO fileno

//--------------------------------------------------
// Module-scope file stream for reading/writing reservation records
static std::fstream reservationFile;

//--------------------------------------------------
// Path of the binary file in use (needed for re-opening after truncation)
static std::string filePath;

//--------------------------------------------------
// Opens the binary reservation file for read/write access.
// If file does not exist, it is created. Returns true on success.
bool open(const std::string &filename)
{
    filePath = filename;

    reservationFile.open(filePath, std::ios::in | std::ios::out | std::ios::binary);

    if (!reservationFile.is_open())
    {
        reservationFile.clear();
        reservationFile.open(filePath, std::ios::out | std::ios::binary);
        reservationFile.close();
        reservationFile.open(filePath, std::ios::in | std::ios::out | std::ios::binary);
    }

    return reservationFile.is_open();
}

//--------------------------------------------------
// Closes the currently open reservation file if open.
void close()
{
    if (reservationFile.is_open())
        reservationFile.close();
}

//--------------------------------------------------
// Saves a reservation record to file. If a matching record
// exists, it is overwritten. Otherwise, the record is appended.
// Returns true if successful.
bool saveReservation(const ReservationRecord &record)
{
    if (!reservationFile.is_open())
        return false;

    ReservationRecord existing;
    long pos = 0;
    reservationFile.clear();
    reservationFile.seekg(0);

    // Search for existing matching record
    while (reservationFile.read(reinterpret_cast<char *>(&existing), sizeof(ReservationRecord)))
    {
        if (std::strncmp(existing.licensePlate, record.licensePlate, LICENSE_PLATE_MAX) == 0 &&
            std::strncmp(existing.sailingID, record.sailingID, SAILING_ID_MAX) == 0)
        {
            reservationFile.seekp(pos);
            reservationFile.write(reinterpret_cast<const char *>(&record), sizeof(ReservationRecord));
            return reservationFile.good(); // confirm successful write
        }
        pos = reservationFile.tellg();
    }

    // Append to end if not found
    reservationFile.clear();
    reservationFile.seekp(0, std::ios::end);
    reservationFile.write(reinterpret_cast<const char *>(&record), sizeof(ReservationRecord));
    return reservationFile.good(); // confirm successful append
}

//--------------------------------------------------
// Retrieves a reservation record matching license plate and sailing ID.
// Loads the result into 'record'. Returns true if found, false otherwise.
bool getReservation(const std::string &licensePlate,
                    const std::string &sailingID,
                    ReservationRecord &record)
{
    if (!reservationFile.is_open())
        return false;

    reservationFile.clear();
    reservationFile.seekg(0);

    while (reservationFile.read(reinterpret_cast<char *>(&record), sizeof(ReservationRecord)))
    {
        if (std::strncmp(record.licensePlate, licensePlate.c_str(), LICENSE_PLATE_MAX) == 0 &&
            std::strncmp(record.sailingID, sailingID.c_str(), SAILING_ID_MAX) == 0)
        {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------
// Returns true if a reservation matching the given license plate
// and sailing ID exists in the file.
bool exists(const std::string &licensePlate, const std::string &sailingID)
{
    ReservationRecord dummy;
    return getReservation(licensePlate, sailingID, dummy);
}

//--------------------------------------------------
// Deletes a reservation record by rewriting all other records
// to a truncated file. Returns true if deletion succeeded.
bool deleteReservation(const std::string &licensePlate, const std::string &sailingID)
{
    if (!reservationFile.is_open())
        return false;

    std::vector<ReservationRecord> all;
    ReservationRecord rec;
    reservationFile.clear();
    reservationFile.seekg(0);

    // GOAL: Read all records that are NOT being deleted
    while (reservationFile.read(reinterpret_cast<char *>(&rec), sizeof(ReservationRecord)))
    {
        if (!(std::strncmp(rec.licensePlate, licensePlate.c_str(), LICENSE_PLATE_MAX) == 0 &&
              std::strncmp(rec.sailingID, sailingID.c_str(), SAILING_ID_MAX) == 0))
        {
            all.push_back(rec);
        }
    }

    // Rewrite all remaining records
    reservationFile.close(); // close before overwriting
    std::ofstream truncFile(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
    for (const auto &r : all)
    {
        truncFile.write(reinterpret_cast<const char *>(&r), sizeof(ReservationRecord));
    }
    truncFile.close();

    // Reopen the file for further I/O
    reservationFile.open(filePath, std::ios::in | std::ios::out | std::ios::binary);
    return true;
}

//--------------------------------------------------
// Retrieves all reservation records that match the given sailing ID.
// Returns them in a vector.
std::vector<ReservationRecord> getAllOnSailing(const std::string &sailingID)
{
    std::vector<ReservationRecord> results;
    if (!reservationFile.is_open())
        return results;

    ReservationRecord rec;
    reservationFile.clear();
    reservationFile.seekg(0);

    while (reservationFile.read(reinterpret_cast<char *>(&rec), sizeof(ReservationRecord)))
    {
        if (std::strncmp(rec.sailingID, sailingID.c_str(), SAILING_ID_MAX) == 0)
        {
            results.push_back(rec);
        }
    }

    return results;
}

//--------------------------------------------------
// Retrieves all reservation records that match the given license plate.
// Returns them in a vector.
std::vector<ReservationRecord> getAllWithVehicle(const std::string &licensePlate)
{
    std::vector<ReservationRecord> results;
    if (!reservationFile.is_open())
        return results;

    ReservationRecord rec;
    reservationFile.clear();
    reservationFile.seekg(0);

    while (reservationFile.read(reinterpret_cast<char *>(&rec), sizeof(ReservationRecord)))
    {
        if (std::strncmp(rec.licensePlate, licensePlate.c_str(), LICENSE_PLATE_MAX) == 0)
        {
            results.push_back(rec);
        }
    }

    return results;
}