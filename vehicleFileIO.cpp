// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
// Implements file I/O operations for Vehicle records, including
// opening, closing, querying, retrieving, and saving data to 
// persistent storage.
// Handles vehicle database operations with proper error 
// checking and data validation.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/23 - James Nguyen
//          - Initial draft for vehicle specification.
// Rev. 2 - 2025/08/05 - James Nguyen
//          - Finalized interface and aligned with .cpp 
//          implementation.
//************************************************************


#include "vehicleFileIO.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

//--------------------------------------------------
// Constants for file operations
//--------------------------------------------------
const string VEHICLE_DATA_FILE = "vehicles.dat";

//--------------------------------------------------
// Binary record structure for Vehicle data
//--------------------------------------------------
struct VehicleRecord {
    char licence[11];    // License plate
    char phone[15];      // Phone number
    float length;        // Vehicle length
    float height;        // Vehicle height
    bool special;        // Special vehicle flag
};

//--------------------------------------------------
// Helper functions for binary data serialization
//--------------------------------------------------
namespace {
    
    // Convert Vehicle object to binary record
    VehicleRecord vehicleToBinaryRecord(const Vehicle& vehicle, const string& licence, const string& phone) {
        VehicleRecord record;
        
        // Clear the record
        memset(&record, 0, sizeof(VehicleRecord));
        
        // Copy data to record
        strncpy(record.licence, licence.c_str(), 10);
        record.licence[10] = '\0';
        
        strncpy(record.phone, phone.c_str(), 14);
        record.phone[14] = '\0';
        
        record.length = vehicle.getLength();
        record.height = vehicle.getHeight();
        record.special = vehicle.isSpecial();
        
        return record;
    }
    
    // Convert binary record to Vehicle object
    Vehicle binaryRecordToVehicle(const VehicleRecord& record, string& licence, string& phone) {
        Vehicle vehicle;
        
        licence = string(record.licence);
        phone = string(record.phone);
        
        vehicle.initialize(record.licence, record.phone, record.length, record.height);
        
        return vehicle;
    }
    
} // end anonymous namespace

//--------------------------------------------------
// FileIOforVehicle class implementation
//--------------------------------------------------

bool FileIOforVehicle::open() {
    try {
        // Try to open file in binary read/write mode
        data.open(VEHICLE_DATA_FILE, ios::in | ios::out | ios::binary);
        
        if (!data.is_open()) {
            // File doesn't exist, create it
            data.clear();
            data.open(VEHICLE_DATA_FILE, ios::out | ios::binary);
            if (!data.is_open()) {
                cerr << "Error: Cannot create vehicle data file." << endl;
                return false;
            }
            data.close();
            
            // Reopen in binary read/write mode
            data.open(VEHICLE_DATA_FILE, ios::in | ios::out | ios::binary);
            if (!data.is_open()) {
                cerr << "Error: Cannot open vehicle data file for read/write." << endl;
                return false;
            }
        }
        
        return true;
    } catch (const exception& e) {
        cerr << "Exception in FileIOforVehicle::open(): " << e.what() << endl;
        return false;
    }
}

bool FileIOforVehicle::close() {
    try {
        if (data.is_open()) {
            data.close();
        }
        return !data.is_open();
    } catch (const exception& e) {
        cerr << "Exception in FileIOforVehicle::close(): " << e.what() << endl;
        return false;
    }
}

bool FileIOforVehicle::exists(const string &licence) {
    if (!data.is_open()) {
        return false;
    }
    
    try {
        // Reset file position
        data.clear();
        data.seekg(0, ios::beg);
        
        VehicleRecord record;
        while (data.read(reinterpret_cast<char*>(&record), sizeof(VehicleRecord))) {
            if (string(record.licence) == licence) {
                return true;
            }
        }
        
        return false;
    } catch (const exception& e) {
        cerr << "Exception in FileIOforVehicle::exists(): " << e.what() << endl;
        return false;
    }
}

vector<Vehicle> FileIOforVehicle::getAllVehicles() {
    vector<Vehicle> vehicles;
    
    if (!data.is_open()) {
        return vehicles;
    }
    
    try {
        // Reset file position
        data.clear();
        data.seekg(0, ios::beg);
        
        VehicleRecord record;
        while (data.read(reinterpret_cast<char*>(&record), sizeof(VehicleRecord))) {
            string licence, phone;
            Vehicle vehicle = binaryRecordToVehicle(record, licence, phone);
            vehicles.push_back(vehicle);
        }
        
        return vehicles;
    } catch (const exception& e) {
        cerr << "Exception in FileIOforVehicle::getAllVehicles(): " << e.what() << endl;
        return vehicles;
    }
}

Vehicle FileIOforVehicle::getVehicle(const string &licence) {
    Vehicle vehicle;
    
    if (!data.is_open()) {
        return vehicle;
    }
    
    try {
        // Reset file position
        data.clear();
        data.seekg(0, ios::beg);
        
        VehicleRecord record;
        while (data.read(reinterpret_cast<char*>(&record), sizeof(VehicleRecord))) {
            if (string(record.licence) == licence) {
                string licenceStr, phone;
                return binaryRecordToVehicle(record, licenceStr, phone);
            }
        }
        
        return vehicle; // Empty vehicle if not found
    } catch (const exception& e) {
        cerr << "Exception in FileIOforVehicle::getVehicle(): " << e.what() << endl;
        return vehicle;
    }
}

bool FileIOforVehicle::saveVehicle(const Vehicle &v) {
    // For basic save, we need license and phone. 
    // This is a simplified version - the caller should use saveVehicleWithData
    return saveVehicleWithData(v, "UNKNOWN", v.getPhone());
}

bool FileIOforVehicle::saveVehicleWithData(const Vehicle &vehicle, 
                                           const string &licence, 
                                           const string &phone) {
    if (!data.is_open()) {
        return false;
    }
    
    try {
        VehicleRecord record = vehicleToBinaryRecord(vehicle, licence, phone);
        
        // Check if vehicle already exists
        if (exists(licence)) {
            // Update existing record
            data.clear();
            data.seekg(0, ios::beg);
            
            VehicleRecord tempRecord;
            streampos position = 0;
            
            while (data.read(reinterpret_cast<char*>(&tempRecord), sizeof(VehicleRecord))) {
                if (string(tempRecord.licence) == licence) {
                    // Found the record, overwrite it
                    data.seekp(position);
                    data.write(reinterpret_cast<const char*>(&record), sizeof(VehicleRecord));
                    data.flush();
                    return true;
                }
                position = data.tellg();
            }
        } else {
            // Append new record
            data.clear();
            data.seekp(0, ios::end);
            data.write(reinterpret_cast<const char*>(&record), sizeof(VehicleRecord));
            data.flush();
            return true;
        }
        
        return false;
    } catch (const exception& e) {
        cerr << "Exception in FileIOforVehicle::saveVehicleWithData(): " << e.what() << endl;
        return false;
    }
}

bool FileIOforVehicle::deleteVehicle(const string &licence) {
    if (!data.is_open()) {
        return false;
    }
    
    try {
        // Read all records except the one to delete
        vector<VehicleRecord> records;
        data.clear();
        data.seekg(0, ios::beg);
        
        VehicleRecord record;
        while (data.read(reinterpret_cast<char*>(&record), sizeof(VehicleRecord))) {
            if (string(record.licence) != licence) {
                records.push_back(record);
            }
        }
        
        // Close and reopen file to truncate
        data.close();
        data.open(VEHICLE_DATA_FILE, ios::out | ios::binary | ios::trunc);
        
        // Write back all records except the deleted one
        for (const auto& rec : records) {
            data.write(reinterpret_cast<const char*>(&rec), sizeof(VehicleRecord));
        }
        
        data.close();
        
        // Reopen in read/write mode
        data.open(VEHICLE_DATA_FILE, ios::in | ios::out | ios::binary);
        
        return true;
    } catch (const exception& e) {
        cerr << "Exception in FileIOforVehicle::deleteVehicle(): " << e.what() << endl;
        return false;
    }
}

bool FileIOforVehicle::getVehicleWithData(const string &licence, 
                                          Vehicle &vehicle, 
                                          string &phone) {
    if (!data.is_open()) {
        return false;
    }
    
    try {
        // Reset file position
        data.clear();
        data.seekg(0, ios::beg);
        
        VehicleRecord record;
        while (data.read(reinterpret_cast<char*>(&record), sizeof(VehicleRecord))) {
            if (string(record.licence) == licence) {
                string licenceStr;
                vehicle = binaryRecordToVehicle(record, licenceStr, phone);
                return true;
            }
        }
        
        return false;
    } catch (const exception& e) {
        cerr << "Exception in FileIOforVehicle::getVehicleWithData(): " << e.what() << endl;
        return false;
    }
}
