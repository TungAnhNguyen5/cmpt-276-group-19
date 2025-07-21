//************************************************************
// FILE: ReservationFileIO.cpp
//************************************************************
// Implements: ReservationFileIO.h
// Description:
//   Handles persistent binary file storage for reservations.
//************************************************************

#include "../header/reservationFileIO.h"
#include "../header/reservation.h"
#include <fstream>
#include <vector>
#include <cstring>
#include <unistd.h>
#define TRUNCATE ftruncate
#define FILENO fileno

static std::fstream reservationFile; // file to read and write
static std::string filePath;         // path to file to read and write

bool open(const std::string &filename)
{
    filePath = filename;
    // Try to open file for reading and writing in binary mode
    reservationFile.open(filePath, std::ios::in | std::ios::out | std::ios::binary);

    // If open fails
    if (!reservationFile.is_open())
    {
        // Clear any error state flags
        reservationFile.clear();

        // Create the file by opening it with 'out' and 'binary' flags
        reservationFile.open(filePath, std::ios::out | std::ios::binary);
        reservationFile.close(); // Close it right after creating

        // Reopen the newly created file for reading and writing
        reservationFile.open(filePath, std::ios::in | std::ios::out | std::ios::binary);
    }

    // Return state of whether the file is now successfully open
    return reservationFile.is_open();
}

void close()
{
    if (reservationFile.is_open())
        reservationFile.close();
}

bool saveReservation(const ReservationRecord &record)
{
    if (!reservationFile.is_open())
        return false;

    ReservationRecord existing;
    long pos = 0;
    reservationFile.clear();
    reservationFile.seekg(0);

    while (reservationFile.read(reinterpret_cast<char *>(&existing), sizeof(ReservationRecord)))
    {
        if (std::strncmp(existing.licensePlate, record.licensePlate, LICENSE_PLATE_MAX) == 0 &&
            std::strncmp(existing.sailingID, record.sailingID, SAILING_ID_MAX) == 0)
        {
            reservationFile.seekp(pos);
            reservationFile.write(reinterpret_cast<const char *>(&record), sizeof(ReservationRecord));
            return true;
        }
        pos = reservationFile.tellg();
    }

    reservationFile.clear();
    reservationFile.seekp(0, std::ios::end);
    reservationFile.write(reinterpret_cast<const char *>(&record), sizeof(ReservationRecord));
    return true;
}

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

bool exists(const std::string &licensePlate, const std::string &sailingID)
{
    ReservationRecord dummy;
    return getReservation(licensePlate, sailingID, dummy);
}

bool deleteReservation(const std::string &licensePlate, const std::string &sailingID)
{
    if (!reservationFile.is_open())
        return false;

    std::vector<ReservationRecord> all;
    ReservationRecord rec;
    reservationFile.clear();
    reservationFile.seekg(0);

    while (reservationFile.read(reinterpret_cast<char *>(&rec), sizeof(ReservationRecord)))
    {
        if (!(std::strncmp(rec.licensePlate, licensePlate.c_str(), LICENSE_PLATE_MAX) == 0 &&
              std::strncmp(rec.sailingID, sailingID.c_str(), SAILING_ID_MAX) == 0))
        {
            all.push_back(rec);
        }
    }

    std::ofstream truncFile(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
    for (const auto &r : all)
    {
        truncFile.write(reinterpret_cast<const char *>(&r), sizeof(ReservationRecord));
    }
    truncFile.close();

    reservationFile.open(filePath, std::ios::in | std::ios::out | std::ios::binary);
    return true;
}

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
