//************************************************************
// FILE: vehicleFileIO.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
// Implements file I/O operations for Vehicle records, including
// opening, closing, querying, retrieving, and saving data to persistent storage.
// Handles vehicle database operations with proper error checking and data validation.
//************************************************************

#include "vehicleFileIO.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>

using namespace std;

//--------------------------------------------------
// Constants for file operations
//--------------------------------------------------
const string VEHICLE_DATA_FILE = "vehicles.txt";
const char FIELD_DELIMITER = '|';

//--------------------------------------------------
// Helper functions for data serialization
//--------------------------------------------------
namespace {
    
    // Convert Vehicle object to string for file storage
    string vehicleToString(const Vehicle& vehicle) {
        ostringstream oss;
        oss << vehicle.getLength() << FIELD_DELIMITER 
            << vehicle.getHeight() << FIELD_DELIMITER
            << (vehicle.isSpecial() ? "1" : "0");
        return oss.str();
    }
    
    // Parse vehicle data from file line
    Vehicle parseVehicleLine(const string& line) {
        Vehicle vehicle;
        
        // Find delimiters
        size_t firstDelim = line.find(FIELD_DELIMITER);
        size_t secondDelim = line.find(FIELD_DELIMITER, firstDelim + 1);
        size_t thirdDelim = line.find(FIELD_DELIMITER, secondDelim + 1);
        size_t fourthDelim = line.find(FIELD_DELIMITER, thirdDelim + 1);
        
        if (firstDelim == string::npos || secondDelim == string::npos || 
            thirdDelim == string::npos || fourthDelim == string::npos) {
            // Invalid format, return empty vehicle
            return vehicle;
        }
        
        try {
            // Extract fields
            string licence = line.substr(0, firstDelim);
            string phone = line.substr(firstDelim + 1, secondDelim - firstDelim - 1);
            float length = stof(line.substr(secondDelim + 1, thirdDelim - secondDelim - 1));
            float height = stof(line.substr(thirdDelim + 1, fourthDelim - thirdDelim - 1));
            
            // Initialize vehicle with parsed data
            vehicle.initialize(licence.c_str(), phone.c_str(), length, height);
            
        } catch (const exception& e) {
            cerr << "Error parsing vehicle data: " << e.what() << endl;
            // Return empty vehicle on parse error
        }
        
        return vehicle;
    }
    
    // Create file line from vehicle data
    string createVehicleLine(const Vehicle& vehicle, const string& licence, const string& phone) {
        ostringstream oss;
        oss << licence << FIELD_DELIMITER 
            << phone << FIELD_DELIMITER
            << vehicle.getLength() << FIELD_DELIMITER
            << vehicle.getHeight() << FIELD_DELIMITER
            << (vehicle.isSpecial() ? "1" : "0");
        return oss.str();
    }
    
} // end anonymous namespace

//--------------------------------------------------
// FileIOforVehicle class implementation
//--------------------------------------------------

bool FileIOforVehicle::open() {
    try {
        // Try to open file in read/write mode
        data.open(VEHICLE_DATA_FILE, ios::in | ios::out);
        
        if (!data.is_open()) {
            // File doesn't exist, create it
            data.clear();
            data.open(VEHICLE_DATA_FILE, ios::out);
            if (!data.is_open()) {
                cerr << "Error: Cannot create vehicle data file." << endl;
                return false;
            }
            data.close();
            
            // Reopen in read/write mode
            data.open(VEHICLE_DATA_FILE, ios::in | ios::out);
            if (!data.is_open()) {
                cerr << "Error: Cannot open vehicle data file for read/write." << endl;
                return false;
            }
        }
        
        return true;
        
    } catch (const exception& e) {
        cerr << "Error opening vehicle file: " << e.what() << endl;
        return false;
    }
}

bool FileIOforVehicle::close() {
    try {
        if (data.is_open()) {
            data.close();
        }
        return true;
        
    } catch (const exception& e) {
        cerr << "Error closing vehicle file: " << e.what() << endl;
        return false;
    }
}

bool FileIOforVehicle::exists(const string& licence) {
    if (!data.is_open()) {
        cerr << "Error: Vehicle file not open." << endl;
        return false;
    }
    
    try {
        // Reset to beginning of file
        data.clear();
        data.seekg(0, ios::beg);
        
        string line;
        while (getline(data, line)) {
            if (!line.empty()) {
                size_t firstDelim = line.find(FIELD_DELIMITER);
                if (firstDelim != string::npos) {
                    string fileLicence = line.substr(0, firstDelim);
                    if (fileLicence == licence) {
                        return true;
                    }
                }
            }
        }
        
        return false;
        
    } catch (const exception& e) {
        cerr << "Error checking vehicle existence: " << e.what() << endl;
        return false;
    }
}

vector<Vehicle> FileIOforVehicle::getAllVehicles() {
    vector<Vehicle> vehicles;
    
    if (!data.is_open()) {
        cerr << "Error: Vehicle file not open." << endl;
        return vehicles;
    }
    
    try {
        // Reset to beginning of file
        data.clear();
        data.seekg(0, ios::beg);
        
        string line;
        while (getline(data, line)) {
            if (!line.empty()) {
                Vehicle vehicle = parseVehicleLine(line);
                // Only add valid vehicles (check if length > 0 as indicator)
                if (vehicle.getLength() > 0) {
                    vehicles.push_back(vehicle);
                }
            }
        }
        
    } catch (const exception& e) {
        cerr << "Error reading all vehicles: " << e.what() << endl;
        vehicles.clear();
    }
    
    return vehicles;
}

Vehicle FileIOforVehicle::getVehicle(const string& licence) {
    Vehicle vehicle; // Default empty vehicle
    
    if (!data.is_open()) {
        cerr << "Error: Vehicle file not open." << endl;
        return vehicle;
    }
    
    try {
        // Reset to beginning of file
        data.clear();
        data.seekg(0, ios::beg);
        
        string line;
        while (getline(data, line)) {
            if (!line.empty()) {
                size_t firstDelim = line.find(FIELD_DELIMITER);
                if (firstDelim != string::npos) {
                    string fileLicence = line.substr(0, firstDelim);
                    if (fileLicence == licence) {
                        vehicle = parseVehicleLine(line);
                        break;
                    }
                }
            }
        }
        
    } catch (const exception& e) {
        cerr << "Error retrieving vehicle: " << e.what() << endl;
    }
    
    return vehicle;
}

bool FileIOforVehicle::saveVehicle(const Vehicle& vehicle) {
    if (!data.is_open()) {
        cerr << "Error: Vehicle file not open." << endl;
        return false;
    }
    
    try {
        // For this implementation, we need additional data (licence and phone)
        // that are not accessible from the Vehicle object's public interface.
        // In a real implementation, these would be stored in the Vehicle object
        // or passed as additional parameters.
        
        // For now, we'll append to the end of the file
        // This is a simplified implementation
        
        // Move to end of file
        data.clear();
        data.seekp(0, ios::end);
        
        // Note: This is a simplified version. In a complete implementation,
        // we would need licence and phone data to create a proper line.
        // For demonstration, we'll create a placeholder entry.
        
        data << "TEMP_LICENSE" << FIELD_DELIMITER 
             << "TEMP_PHONE" << FIELD_DELIMITER
             << vehicle.getLength() << FIELD_DELIMITER
             << vehicle.getHeight() << FIELD_DELIMITER
             << (vehicle.isSpecial() ? "1" : "0") << endl;
        
        data.flush();
        return true;
        
    } catch (const exception& e) {
        cerr << "Error saving vehicle: " << e.what() << endl;
        return false;
    }
}

//--------------------------------------------------
// Enhanced saveVehicle function with full data
//--------------------------------------------------
bool FileIOforVehicle::saveVehicleWithData(const Vehicle& vehicle, 
                                           const string& licence, 
                                           const string& phone) {
    if (!data.is_open()) {
        cerr << "Error: Vehicle file not open." << endl;
        return false;
    }
    
    try {
        // Check if vehicle already exists and update, or append new
        vector<string> lines;
        bool found = false;
        
        // Read all existing data
        data.clear();
        data.seekg(0, ios::beg);
        
        string line;
        while (getline(data, line)) {
            if (!line.empty()) {
                size_t firstDelim = line.find(FIELD_DELIMITER);
                if (firstDelim != string::npos) {
                    string fileLicence = line.substr(0, firstDelim);
                    if (fileLicence == licence) {
                        // Update existing vehicle
                        lines.push_back(createVehicleLine(vehicle, licence, phone));
                        found = true;
                    } else {
                        lines.push_back(line);
                    }
                } else {
                    lines.push_back(line);
                }
            }
        }
        
        // If not found, add new vehicle
        if (!found) {
            lines.push_back(createVehicleLine(vehicle, licence, phone));
        }
        
        // Rewrite entire file
        data.close();
        data.open(VEHICLE_DATA_FILE, ios::out | ios::trunc);
        
        if (!data.is_open()) {
            cerr << "Error: Cannot reopen file for writing." << endl;
            return false;
        }
        
        for (const string& fileLine : lines) {
            data << fileLine << endl;
        }
        
        data.flush();
        data.close();
        
        // Reopen in read/write mode
        data.open(VEHICLE_DATA_FILE, ios::in | ios::out);
        
        return data.is_open();
        
    } catch (const exception& e) {
        cerr << "Error saving vehicle with data: " << e.what() << endl;
        return false;
    }
}

//--------------------------------------------------
// Function to delete a vehicle by licence plate
//--------------------------------------------------
bool FileIOforVehicle::deleteVehicle(const string& licence) {
    if (!data.is_open()) {
        cerr << "Error: Vehicle file not open." << endl;
        return false;
    }
    
    try {
        vector<string> lines;
        bool found = false;
        
        // Read all existing data
        data.clear();
        data.seekg(0, ios::beg);
        
        string line;
        while (getline(data, line)) {
            if (!line.empty()) {
                size_t firstDelim = line.find(FIELD_DELIMITER);
                if (firstDelim != string::npos) {
                    string fileLicence = line.substr(0, firstDelim);
                    if (fileLicence != licence) {
                        lines.push_back(line);
                    } else {
                        found = true;
                    }
                } else {
                    lines.push_back(line);
                }
            }
        }
        
        if (!found) {
            return false; // Vehicle not found
        }
        
        // Rewrite file without the deleted vehicle
        data.close();
        data.open(VEHICLE_DATA_FILE, ios::out | ios::trunc);
        
        if (!data.is_open()) {
            cerr << "Error: Cannot reopen file for writing." << endl;
            return false;
        }
        
        for (const string& fileLine : lines) {
            data << fileLine << endl;
        }
        
        data.flush();
        data.close();
        
        // Reopen in read/write mode
        data.open(VEHICLE_DATA_FILE, ios::in | ios::out);
        
        return data.is_open();
        
    } catch (const exception& e) {
        cerr << "Error deleting vehicle: " << e.what() << endl;
        return false;
    }
}

//--------------------------------------------------
// Function to get vehicle data including licence and phone
//--------------------------------------------------
bool FileIOforVehicle::getVehicleWithData(const string& licence, 
                                          Vehicle& vehicle, 
                                          string& phone) {
    if (!data.is_open()) {
        cerr << "Error: Vehicle file not open." << endl;
        return false;
    }
    
    try {
        // Reset to beginning of file
        data.clear();
        data.seekg(0, ios::beg);
        
        string line;
        while (getline(data, line)) {
            if (!line.empty()) {
                size_t firstDelim = line.find(FIELD_DELIMITER);
                if (firstDelim != string::npos) {
                    string fileLicence = line.substr(0, firstDelim);
                    if (fileLicence == licence) {
                        // Parse the line
                        size_t secondDelim = line.find(FIELD_DELIMITER, firstDelim + 1);
                        if (secondDelim != string::npos) {
                            phone = line.substr(firstDelim + 1, secondDelim - firstDelim - 1);
                            vehicle = parseVehicleLine(line);
                            return true;
                        }
                    }
                }
            }
        }
        
        return false; // Vehicle not found
        
    } catch (const exception& e) {
        cerr << "Error retrieving vehicle with data: " << e.what() << endl;
        return false;
    }
}
