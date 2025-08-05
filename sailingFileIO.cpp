// FILE: sailingFileIO.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/08/05
//************************************************************
// PURPOSE:
// Implements binary file I/O operations for Sailing records
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/24 - Nathan Miller
//          - Initial .cpp implementation based on header specification.
// Rev. 2 - 2025/08/05 - Nathan Miller
//          - Fully debugged final release version.
//************************************************************

#include "sailingFileIO.h"
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

const string FILE_NAME = "sailingData.dat";
fstream sailingFileIO::file;

//--------------------------------------------------
// Binary record structure for Sailing data
//--------------------------------------------------
struct SailingRecord {
    char sailingID[10];    // Sailing ID
    char vesselID[26];     // Vessel ID
    int lcll;              // Load car length limit
    int hcll;              // Height car length limit
    float lrl;             // Load remaining length
    float hrl;             // Height remaining length
};

//--------------------------------------------------
// Helper functions for binary data serialization
//--------------------------------------------------

// Convert Sailing object to binary record
SailingRecord sailingToBinaryRecord(const Sailing& sailing) {
    SailingRecord record;
    
    // Clear the record
    memset(&record, 0, sizeof(SailingRecord));
    
    // Copy data to record
    strncpy(record.sailingID, sailing.getSailingID(), 9);
    record.sailingID[9] = '\0';
    
    strncpy(record.vesselID, sailing.getVesselID(), 25);
    record.vesselID[25] = '\0';
    
    record.lcll = sailing.getLCLL();
    record.hcll = sailing.getHCLL();
    record.lrl = sailing.getLRL();
    record.hrl = sailing.getHRL();
    
    return record;
}

// Convert binary record to Sailing object
Sailing binaryRecordToSailing(const SailingRecord& record) {
    Sailing sailing;
    
    // Create a line string for the existing createSailing method
    string line = string(record.sailingID) + "|" + 
                  string(record.vesselID) + "|" +
                  to_string(record.lcll) + "|" +
                  to_string(record.hcll) + "|" +
                  to_string(record.lrl) + "|" +
                  to_string(record.hrl);
    
    sailing.createSailing(line);
    
    return sailing;
}

bool sailingFileIO::closeFile()
{
    if (file.is_open()) {
        file.close();
    }
    return !file.is_open();
}

bool sailingFileIO::openFile()
{
    file.open(FILE_NAME, ios::in | ios::out | ios::binary);
    // if opening doesn't work, creates the file
    if (!file.is_open())
    {
        createFile();
        file.open(FILE_NAME, ios::in | ios::out | ios::binary);
    }
    return file.is_open();
}

void sailingFileIO::reset()
{
    file.clear();
    file.seekg(0);
}

Sailing sailingFileIO::getSailing(const char *sid)
{
    reset();
    
    SailingRecord record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(SailingRecord))) {
        if (string(record.sailingID) == string(sid)) {
            return binaryRecordToSailing(record);
        }
    }
    
    // Return empty sailing if not found
    Sailing emptySailing;
    return emptySailing;
}

Sailing *sailingFileIO::getNextFive()
{
    Sailing *fiveSailings = new Sailing[5];
    
    // Initialize all sailings to empty state
    for (int i = 0; i < 5; i++) {
        // Create an empty sailing with null-terminated sailingID
        Sailing emptySailing;
        // Ensure the sailingID is properly null-terminated to indicate empty
        memset(&emptySailing, 0, sizeof(Sailing));
        fiveSailings[i] = emptySailing;
    }
    
    SailingRecord record;
    for (int i = 0; i < 5; i++) {
        if (file.read(reinterpret_cast<char*>(&record), sizeof(SailingRecord))) {
            fiveSailings[i] = binaryRecordToSailing(record);
        } else {
            // If we can't read more records, break early
            break;
        }
    }
    
    return fiveSailings;
}

bool sailingFileIO::exists(const char *sid)
{
    reset();
    
    SailingRecord record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(SailingRecord))) {
        if (string(record.sailingID) == string(sid)) {
            return true;
        }
    }
    return false;
}

bool sailingFileIO::saveSailing(const Sailing s)
{
    if (!file.is_open()) {
        cout << "file not open";
        return false;
    }
    
    try {
        SailingRecord record = sailingToBinaryRecord(s);
        
        // Check if sailing already exists
        if (exists(s.getSailingID())) {
            // Update existing record
            reset();
            SailingRecord tempRecord;
            streampos position = 0;
            
            while (file.read(reinterpret_cast<char*>(&tempRecord), sizeof(SailingRecord))) {
                if (string(tempRecord.sailingID) == string(s.getSailingID())) {
                    // Found the record, overwrite it
                    file.seekp(position);
                    file.write(reinterpret_cast<const char*>(&record), sizeof(SailingRecord));
                    file.flush();
                    return true;
                }
                position = file.tellg();
            }
        } else {
            // Append new record
            file.clear();
            file.seekp(0, ios::end);
            file.write(reinterpret_cast<const char*>(&record), sizeof(SailingRecord));
            file.flush();
            return true;
        }
        
        return false;
    } catch (const exception& e) {
        cerr << "Exception in saveSailing: " << e.what() << endl;
        return false;
    }
}

bool sailingFileIO::deleteSailing(const char *sid)
{
    if (!file.is_open()) {
        return false;
    }
    
    try {
        // Read all records except the one to delete
        vector<SailingRecord> records;
        reset();
        
        SailingRecord record;
        while (file.read(reinterpret_cast<char*>(&record), sizeof(SailingRecord))) {
            if (string(record.sailingID) != string(sid)) {
                records.push_back(record);
            }
        }
        
        // Close and reopen file to truncate
        file.close();
        file.open(FILE_NAME, ios::out | ios::binary | ios::trunc);
        
        // Write back all records except the deleted one
        for (const auto& rec : records) {
            file.write(reinterpret_cast<const char*>(&rec), sizeof(SailingRecord));
        }
        
        file.close();
        
        // Reopen in read/write mode
        file.open(FILE_NAME, ios::in | ios::out | ios::binary);
        
        return true;
    } catch (const exception& e) {
        cerr << "Exception in deleteSailing: " << e.what() << endl;
        return false;
    }
}

void sailingFileIO::truncateFile()
{
    // Binary file truncation is handled by the rewrite operations above
    // No need for special truncation logic
}

Sailing sailingFileIO::getLast()
{
    reset();
    Sailing last;
    
    SailingRecord record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(SailingRecord))) {
        last = binaryRecordToSailing(record);
    }
    
    return last;
}

void sailingFileIO::createFile()
{
    ofstream f(FILE_NAME, ios::binary);
    f.close();
}
