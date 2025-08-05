// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/09
//************************************************************
// PURPOSE:
// Declares the interface for file I/O operations on Vehicle 
// records,
// including opening, closing, querying, retrieving, and saving 
// data.
//************************************************************
// USAGE:
// - Instantiate FileIOforVehicle and call open() before any 
// other operations.
// - Use exists(), getVehicle(), or getAllVehicles() for 
// queries.
// - Use saveVehicle() to persist a Vehicle.
// - Call close() when done.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - James Nguyen
//          - Initial draft for vehicle file IO specification.
// Rev. 2 - 2025/08/05 - James Nguyen
//          - Finalized interface and aligned with .cpp
// implementation.
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

// Helper for navigating persistent Vehicle records.
class FileIOforVehicle
{
private:
    std::fstream data;  // file stream for vehicle data

public:
    // Opens the vehicle data file.
    // Returns false if unable to open.
    bool open();

    // Closes the vehicle data file.
    // Returns false if unable to close.
    bool close();

    // Checks if a vehicle with the given license exists.
    // in:  licence – license string to query
    // Returns true if found.
    bool exists(const std::string &licence);

    // Retrieves all vehicles from the data file.
    // out: vector of Vehicle objects (empty if none).
    std::vector<Vehicle> getAllVehicles();

    // Retrieves a single Vehicle by license.
    // in:  licence – license string to retrieve
    // Returns a Vehicle object populated from file.
    Vehicle getVehicle(const std::string &licence);

    // Saves a Vehicle record to the data file.
    // in:  v – Vehicle object to persist
    // Returns true if successful.
    bool saveVehicle(const Vehicle &v);

    // Saves a Vehicle record with complete data to the 
    // data file.
    // in:  vehicle – Vehicle object to persist
    // in:  licence – license plate string
    // in:  phone – phone number string
    // Returns true if successful.
    bool saveVehicleWithData(const Vehicle &vehicle, 
                            const std::string &licence, 
                            const std::string &phone);

    // Deletes a vehicle record by license plate.
    // in:  licence – license plate string to delete
    // Returns true if successful.
    bool deleteVehicle(const std::string &licence);

    // Retrieves a Vehicle with complete data by license.
    // in:  licence – license string to retrieve
    // out: vehicle – Vehicle object to populate
    // out: phone – phone number string
    // Returns true if found.
    bool getVehicleWithData(const std::string &licence, 
                           Vehicle &vehicle, 
                           std::string &phone);
};

#endif // VEHICLE_FILE_IO_H
