//************************************************************
// FILE: VehicleFileIO.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #3)
// TEAM: Group 19
// DATE: 2025/07/09
//************************************************************
// PURPOSE:
// Declares the interface for file I/O operations on Vehicle records,
// including opening, closing, querying, retrieving, and saving data.
//************************************************************
// USAGE:
// - Instantiate FileIOforVehicle and call open() before any other operations.
// - Use exists(), getVehicle(), or getAllVehicles() for queries.
// - Use saveVehicle() to persist a Vehicle.
// - Call close() when done.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter
//************************************************************

#ifndef VEHICLE_FILE_IO_H
#define VEHICLE_FILE_IO_H

#include <fstream>
#include <string>
#include <vector>
#include "vehicle.h"

//--------------------------------------------------
// Helper for navigating persistent Vehicle records.
class FileIOforVehicle
{
private:
    std::fstream data;  // file stream for vehicle data

public:
    //--------------------------------------------------
    // Opens the vehicle data file.
    // Returns false if unable to open.
    bool open();

    //--------------------------------------------------
    // Closes the vehicle data file.
    // Returns false if unable to close.
    bool close();

    //--------------------------------------------------
    // Checks if a vehicle with the given license exists.
    // in:  licence – license string to query
    // Returns true if found.
    bool exists(const std::string &licence);

    //--------------------------------------------------
    // Retrieves all vehicles from the data file.
    // out: vector of Vehicle objects (empty if none).
    std::vector<Vehicle> getAllVehicles();

    //--------------------------------------------------
    // Retrieves a single Vehicle by license.
    // in:  licence – license string to retrieve
    // Returns a Vehicle object populated from file.
    Vehicle getVehicle(const std::string &licence);

    //--------------------------------------------------
    // Saves a Vehicle record to the data file.
    // in:  v – Vehicle object to persist
    // Returns true if successful.
    bool saveVehicle(const Vehicle &v);
};

#endif // VEHICLE_FILE_IO_H
