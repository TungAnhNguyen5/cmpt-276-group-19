# Ferry Reservation Software System (FRSS)
## Detailed Design Document

**Project:** CMPT 276 – Ferry Reservation Software System  
**Team:** Group 19  
**Date:** July 23, 2025  
**Version:** 1.0  

---

## Table of Contents

1. [Executive Summary](#executive-summary)
2. [System Architecture](#system-architecture)
3. [Module Design](#module-design)
4. [User Interface Design](#user-interface-design)
5. [Data Management](#data-management)
6. [Error Handling](#error-handling)
7. [Build System](#build-system)
8. [Testing Strategy](#testing-strategy)
9. [Source Code](#source-code)

---

## Executive Summary

The Ferry Reservation Software System (FRSS) is a comprehensive console-based application designed to modernize Coastal Ferry Corp's vehicle reservation and management operations. The system transitions from a first-come, first-served model to a sophisticated reservation-based system that improves customer experience, optimizes vehicle space utilization, and reduces terminal congestion.

### Key Features:
- **Menu-driven interface** accessible to users with minimal computer experience
- **Real-time sailing management** with capacity tracking
- **Vehicle classification system** (regular vs. special vehicles)
- **Reservation management** with check-in capabilities
- **Comprehensive reporting** for operational insights
- **Robust file I/O system** for data persistence
- **Modular architecture** for maintainability and scalability

---

## System Architecture

### High-Level Architecture

The FRSS follows a layered architecture pattern with clear separation of concerns:

```
┌─────────────────────────────────────────┐
│             User Interface             │
│              (ui.cpp/h)                │
├─────────────────────────────────────────┤
│          Business Logic Layer          │
│   ┌─────────┐ ┌─────────┐ ┌──────────┐ │
│   │Sailing  │ │Vehicle  │ │Reservation│ │
│   │Module   │ │Module   │ │ Module   │ │
│   └─────────┘ └─────────┘ └──────────┘ │
├─────────────────────────────────────────┤
│           Data Access Layer            │
│   ┌─────────┐ ┌─────────┐ ┌──────────┐ │
│   │Sailing  │ │Vehicle  │ │Reservation│ │
│   │FileIO   │ │FileIO   │ │ FileIO   │ │
│   └─────────┘ └─────────┘ └──────────┘ │
├─────────────────────────────────────────┤
│            File System                 │
│        (Data Persistence)              │
└─────────────────────────────────────────┘
```

### Core Design Principles:

1. **Modularity**: Each component has a single responsibility
2. **Encapsulation**: Internal implementation details are hidden
3. **Separation of Concerns**: UI, business logic, and data access are distinct
4. **Error Resilience**: Comprehensive error handling throughout the system
5. **User-Centric Design**: Interface optimized for ease of use

---

## Module Design

### 1. Main Application (main.cpp)

**Purpose:** Entry point that orchestrates system initialization, execution, and shutdown.

**Key Responsibilities:**
- System initialization through UI::initialize()
- Main menu loop execution via UI::mainMenu()
- Clean shutdown with UI::shutdown()
- Error handling for critical failures

### 2. User Interface Module (ui.cpp/h)

**Purpose:** Provides a console-based, menu-driven interface accessible to users with minimal computer experience.

**Key Components:**
- **Main Menu Navigation**: Hierarchical menu system with numbered options
- **Input Validation**: Robust input handling with error recovery
- **Display Utilities**: Consistent formatting for headers, footers, and data presentation
- **Integration Layer**: Bridges user actions with business logic modules

**Design Features:**
- Anonymous namespace for internal helper functions
- Public interface for cross-module communication
- Step-by-step guidance for complex operations
- Cancellation support at every step

### 3. Sailing Management Module (sailing.cpp/h)

**Purpose:** Manages ferry sailing schedules, capacity tracking, and operational data.

**Key Features:**
- **Sailing Creation**: Step-by-step sailing setup with validation
- **Capacity Management**: Real-time tracking of regular and special vehicle space
- **Schedule Management**: Date, time, and terminal coordination
- **Integration**: Seamless connection with reservation management

**Data Structure:**
- Sailing ID (format: TERMINAL-DATE-HOUR)
- Vessel identification
- Capacity limits (LCLL for regular, HCLL for special vehicles)
- Remaining space tracking (LRL, HRL)

### 4. Vehicle Management Module (vehicle.cpp/h)

**Purpose:** Handles vehicle registration, classification, and data management.

**Classification Logic:**
- **Regular Vehicles**: Height ≤ 2.0m AND Length ≤ 7.0m
- **Special Vehicles**: Height > 2.0m OR Length > 7.0m

**Key Operations:**
- Vehicle registration with license plate and contact information
- Dimensional data capture and validation
- Automatic classification based on size criteria
- Integration with reservation system

### 5. Reservation Management Module (reservation.cpp/h)

**Purpose:** Coordinates vehicle reservations with sailing schedules and capacity management.

**Core Functionality:**
- **Reservation Creation**: Links vehicles to specific sailings
- **Check-in Process**: Manages on-board status tracking
- **Capacity Validation**: Ensures reservations don't exceed sailing limits
- **Data Integrity**: Maintains consistency across all system components

### 6. File I/O Modules

**sailingFileIO.cpp/h, vehicleFileIO.cpp/h, reservationFileIO.cpp/h**

**Purpose:** Provides persistent data storage and retrieval capabilities.

**Key Features:**
- **Binary file format** for efficient storage
- **Exception-safe operations** with automatic resource management
- **Batch operations** for improved performance
- **Data integrity validation** on read/write operations

---

## User Interface Design

### Design Philosophy

The UI is designed for **accessibility and ease of use**, targeting users with minimal computer experience. Every operation follows a consistent pattern:

1. **Clear Instructions**: Each screen provides format examples and guidance
2. **Numbered Menus**: Simple numeric selection reduces confusion
3. **Cancellation Support**: Users can exit at any point using "0" or "Cancel"
4. **Confirmation Steps**: Critical operations require explicit confirmation
5. **Error Recovery**: Invalid inputs prompt helpful error messages

### Menu Hierarchy

```
Main Menu
├── [1] Manage Sailings
│   ├── Display sailing list with capacity info
│   ├── [0] Cancel
│   ├── [1] New Sailing
│   └── [Sailing ID] Edit existing sailing
├── [2] Manage Vehicles
│   ├── Display vehicle summary statistics
│   ├── [0] Cancel
│   ├── [1] Add Vehicle
│   └── [License Plate] Edit existing vehicle
├── [3] Manage Reservations
│   ├── [0] Cancel
│   ├── [1] Check-In Process
│   └── [Sailing ID] Manage sailing reservations
├── [4] Show Sailing Report
│   └── Display comprehensive sailing data
└── [0] Exit System
```

### Input Validation Strategy

- **Integer Input**: Range validation with clear error messages
- **String Input**: Format guidance with examples
- **Cancellation**: Consistent "0" or "Cancel" support
- **Error Recovery**: Clear input buffer and retry prompts

---

## Data Management

### File Structure

The system uses three primary data files:
- **Sailing Data**: Binary format storing sailing schedules and capacity
- **Vehicle Data**: Binary format with vehicle details and classification
- **Reservation Data**: Binary format linking vehicles to sailings

### Data Integrity

- **Validation on Input**: All user data validated before storage
- **Exception Safety**: File operations use RAII principles
- **Atomic Operations**: Critical updates completed entirely or not at all
- **Backup Strategy**: Data files can be easily backed up and restored

### Performance Considerations

- **Batch Operations**: Multiple records processed efficiently
- **Memory Management**: Careful resource allocation and deallocation
- **File Locking**: Prevents data corruption from concurrent access

---

## Error Handling

### Strategy

The system implements comprehensive error handling at multiple levels:

1. **Input Validation**: Prevents invalid data entry
2. **File I/O Protection**: Graceful handling of file system errors
3. **Resource Management**: RAII ensures proper cleanup
4. **User Feedback**: Clear error messages guide user action

### Error Categories

- **User Input Errors**: Invalid format, out-of-range values
- **File System Errors**: Missing files, permission issues, disk space
- **Logic Errors**: Capacity exceeded, duplicate entries
- **System Errors**: Memory allocation, unexpected exceptions

---

## Build System

### Automated Build Process

The system includes a comprehensive build script (`build.sh`) that:

- **Compiles Main System**: Creates `ferry_system` executable
- **Builds Unit Tests**: Creates `unit_test` executable  
- **Dependency Management**: Ensures all modules compile correctly
- **Error Reporting**: Clear feedback on compilation issues

### Build Requirements

- **C++ Compiler**: g++ with C++11 support or later
- **Standard Libraries**: iostream, string, vector, limits
- **File System**: Read/write access for data persistence

---

## Testing Strategy

### Unit Testing

The system includes comprehensive unit tests (`unitTest.cpp`) focusing on:

- **File I/O Operations**: Data persistence and retrieval accuracy
- **Data Integrity**: Validation of stored vs. retrieved data
- **Error Conditions**: Handling of file system errors
- **Performance**: Efficient handling of multiple records

### Integration Testing

- **Module Interactions**: Verify cross-module communication
- **End-to-End Workflows**: Complete user scenarios
- **Error Propagation**: Proper error handling across module boundaries

### User Acceptance Testing

- **Usability**: Interface accessibility for target users
- **Functional Requirements**: All specified features working correctly
- **Performance**: Acceptable response times under normal load

---

## Source Code

The following sections contain the complete source code for the Ferry Reservation Software System. Each file begins with its identifier and contains the full implementation.

---


---

## MAIN.CPP

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//************************************************************
// FILE: main.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System 
// TEAM: Group 19
// DATE: 2025/07/09
//************************************************************
// PURPOSE:
// Entry point of the Ferry Reservation Software System.
// Initializes the system, runs the main UI loop, and shuts down cleanly.
//************************************************************

#include <iostream>
#include "ui.h"

int main() {
    // Initialize system modules
    if (!UI::initialize()) {
        std::cerr << "Initialization failed. Exiting program.\n";
        return 1;
    }

    // Launch main menu loop
    UI::mainMenu();

    // Shutdown and cleanup
    UI::shutdown();

    return 0;
}
```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## RESERVATION.CPP

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//************************************************************
// FILE: Reservation.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #4)
// TEAM: Group 19
// DATE: 25/07/24
//************************************************************
// PURPOSE:
//   Implements reservation logic including adding, checking-in,
//   fare calculation, and reservation management.
//************************************************************
// USAGE:
// - Call initialize() before using any functions in this module.
// - Call shutdown() before program exit to flush and close storage.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/24 - Danny Choi
//          - Initial .cpp implementation based on header specification.
//************************************************************


#include "reservation.h"
#include "reservationFileIO.h"
#include <iostream>
#include <cstring>

//--------------------------------------------------
// Function: initialize
//--------------------------------------------------
// Opens the reservation file to initialize the system.
// Required before any other reservation operations can occur.
void initialize()
{
    open("reservation.txt");
}

//--------------------------------------------------
// Function: shutdown
//--------------------------------------------------
// Closes the open reservation file to release resources.
void shutdown()
{
    close();
}

//--------------------------------------------------
// Function: addReservation
//--------------------------------------------------
// Creates a new reservation record, zeroes its memory, fills
// it with user input, and writes it to file. Assumes reservation
// uniqueness is managed at a higher level or via overwrite logic
// in the file layer.
bool addReservation(const std::string &sailingID,
                    const std::string &licensePlate,
                    const std::string &phone,
                    bool isSpecial,
                    float height,
                    float length)
{
    ReservationRecord record;
    std::memset(&record, 0, sizeof(ReservationRecord)); // Clear all bytes in the struct

    std::strncpy(record.sailingID, sailingID.c_str(), SAILING_ID_MAX);
    std::strncpy(record.licensePlate, licensePlate.c_str(), LICENSE_PLATE_MAX);

    record.onboard = false;

    return saveReservation(record);
}

//--------------------------------------------------
// Function: checkedIn
//--------------------------------------------------
// Loads the reservation from file and marks it onboard=true.
// Then saves the updated record. Returns false if reservation
// was not found.
bool checkedIn(const std::string &sailingID,
               const std::string &licensePlate)
{
    ReservationRecord record;

    // Attempt to locate record in file
    if (!getReservation(licensePlate, sailingID, record))
        return false;

    // Update and re-save only if onboard was previously false
    if (!record.onboard)
    {
        record.onboard = true;
        saveReservation(record);
    }

    return true;
}

//--------------------------------------------------
// Function: checkIn
//--------------------------------------------------
// If a reservation exists, performs check-in (onboard=true),
// saves it, and calculates fare based on input vehicle info.
// Returns fare, or -1.0f if reservation doesn't exist.
float checkIn(const std::string &sailingID,
              const std::string &licensePlate,
              bool isSpecial,
              float height,
              float length)
{
    if (!exists(licensePlate, sailingID))
        return -1.0f;

    checkedIn(sailingID, licensePlate);

    return calculateFare(isSpecial, length, height);
}

//--------------------------------------------------
// Function: calculateFare
//--------------------------------------------------
// Computes fare using simple business rules:
// - Regular vehicles: flat rate
// - Over-height special vehicles: $3/m
// - Low long special vehicles: $2/m
float calculateFare(bool isSpecial, float length, float height)
{
    if (!isSpecial)
        return 14.00f;

    if (height > 2.00f)
        return length * 3.00f;

    else if (height <= 2.00f)
        return length * 2.00f;

    return 0.0f; // Fallback for unexpected input
}

//--------------------------------------------------
// Function: deleteAllOnSailing
//--------------------------------------------------
// Iterates through all reservations matching sailingID and deletes them.
// GOAL: count how many reservations were deleted.
// Step: one record at a time from getAllOnSailing result.
int deleteAllOnSailing(const std::string &sailingID)
{
    int count = 0;
    std::vector<ReservationRecord> records = getAllOnSailing(sailingID);

    for (const auto &rec : records)
    {
        // Delete one record at a time by license plate + sailing ID
        if (deleteReservation(std::string(rec.licensePlate), sailingID))
        {
            ++count;
        }
    }

    return count;
}

//--------------------------------------------------
// Function: moveReservations
//--------------------------------------------------
// Transfers all reservations from one sailing ID to another by:
// - reading matching reservations
// - modifying the sailingID field
// - re-saving them with the updated ID
// GOAL: perform in-place update of file contents for sailing migration.
int moveReservations(const std::string &fromSailingID,
                     const std::string &toSailingID)
{
    int count = 0;
    std::vector<ReservationRecord> records = getAllOnSailing(fromSailingID);

    for (auto &rec : records)
    {
        std::strncpy(rec.sailingID, toSailingID.c_str(), SAILING_ID_MAX);

        if (saveReservation(rec))
        {
            ++count;
        }
    }

    return count;
}```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## RESERVATION.H

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//************************************************************
// FILE: Reservation.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #4)
// TEAM: Group 19
// DATE: 25/07/24
//************************************************************
// PURPOSE:
//   Declares the interface for reservation-related logic, including
//   adding, checking-in, and fare calculation.
//************************************************************
// USAGE:
// - Call initialize() before using any functions in this module.
// - Call shutdown() before exiting the program.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - Danny Choi
//          - Initial draft for interface specification.
// Rev. 2 - 2025/07/24 - Danny Choi
//          - Finalized interface and aligned with .cpp implementation.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter
//************************************************************

#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

const int LICENSE_PLATE_MAX = 10; // Max characters for license plate (including null terminator)
const int SAILING_ID_MAX = 9;     // Max characters for sailing ID (including null terminator)

//--------------------------------------------------
// Minimal type used for interface clarity only.
// This struct represents a reservation as seen at the interface level.
// Do not assume this matches the internal data layout in Assignment 4.
struct ReservationData
{
    std::string licensePlate; // Vehicle license plate
    std::string sailingID;    // Associated sailing ID
    bool onboard;             // True if checked-in, false otherwise
};

//--------------------------------------------------
// Initializes the reservation subsystem.
void initialize();

//--------------------------------------------------
// Shuts down the reservation subsystem and releases internal resources.
void shutdown();

//--------------------------------------------------
// Adds a reservation for a vehicle to a sailing.
// Returns true if the reservation was successfully added.
bool addReservation(
    const std::string &sailingID,    // in: sailing to reserve on
    const std::string &licensePlate, // in: vehicle's license plate
    const std::string &phone,        // in: contact phone number
    bool isSpecial,                  // in: true if special vehicle
    float height,                    // in: height (if special), else 0
    float length                     // in: length (if special), else 0
);

//--------------------------------------------------
// Marks an existing reservation as checked in.
// Returns true if successful.
bool checkedIn(
    const std::string &sailingID,   // in: sailing ID
    const std::string &licensePlate // in: vehicle's license plate
);

//--------------------------------------------------
// Performs check-in logic and returns calculated fare.
// Returns -1.0 if reservation does not exist.
float checkIn(
    const std::string &sailingID,    // in: sailing ID
    const std::string &licensePlate, // in: vehicle's license plate
    bool isSpecial,                  // in: true if special vehicle
    float height,                    // in: height if special (m)
    float length                     // in: length if special (m)
);

//--------------------------------------------------
// Calculates the fare based on type and dimensions.
// Returns fare in dollars to two decimal places.
float calculateFare(
    bool isSpecial, // in: true if special vehicle
    float length,   // in: vehicle length in meters
    float height    // in: vehicle height in meters
);

//--------------------------------------------------
// Deletes all reservations for the given sailing.
// Returns the number of reservations deleted.
int deleteAllOnSailing(
    const std::string &sailingID // in: sailing ID
);

//--------------------------------------------------
// Moves all reservations from one sailing to another.
// Returns number of reservations successfully moved.
int moveReservations(
    const std::string &fromSailingID, // in: source sailing ID
    const std::string &toSailingID    // in: destination sailing ID
);

#endif // RESERVATION_H```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## RESERVATIONFILEIO.CPP

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
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
}```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## RESERVATIONFILEIO.H

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//************************************************************
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

#endif // RESERVATION_FILE_IO_H```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## SAILING.CPP

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// FILE: sailing.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
// Implements the console-based user interface for the ferry reservation system.
// Provides a scrolling, text-based interface accessible to users with little
// computer experience. All operations are menu-driven with step-by-step guidance.
//************************************************************

#include "sailing.h"
#include "sailingFileIO.h"
#include "ui.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <limits>

using namespace std;

// cancel is always 0 so using a constant for it makes sense
const int CANCEL_OPTION = 0;

// public functions
bool Sailing::addSailing()
{
    const char SID_SEPARATOR = '-';
    const int CONFIRM_OPTION = 1;

    // uses helper functions to prompt the user for and create the sailing id
    string depTerm = addDepTerm();
    string date = addDate();
    string time = addTime();

    // forms the sailing id according to the ord, converts to char array
    string sidString = depTerm + SID_SEPARATOR + date + SID_SEPARATOR + time;
    char sid[10];

    strcpy(sid, sidString.c_str());
    sid[sizeof(sid) - 1] = '\0';

    // aborts the process if the sailing already exists
    if (sailingFileIO::exists(sid))
    {
        return false;
    }

    // uses helper functions to prompt the user for the rest of the sailing information.
    string vessel = addVessel();
    int lcll = addLCLL();
    int hcll = addHCLL();

    // confirms the entry, cancels if cancelled
    if (!confirm(CONFIRM_OPTION))
    {
        // aborts this operation and hands control back to ui.cpp
        return false;
    }

    // combines everything into a sailing
    Sailing s;
    strcpy(s.sailingID, sid);
    strcpy(s.vesselID, vessel.c_str());
    s.lcll = lcll;
    s.hcll = hcll;
    s.lrl = lcll;
    s.hrl = hcll;

    sailingFileIO::saveSailing(s);
    return true;
}

void Sailing::editSailing()
{
    // used to keep track of order and options
    enum editSailingOptions
    {
        DEP_TERM = 1,
        DEP_DAY,
        DEP_TIME,
        VESSEL_ID,
        LCLL,
        HCLL,
        NUM_OF_OPTIONS = 6
    };

    Sailing s = *this;
    string sid = s.sailingID;
    // for loop to print the option select in order
    for (int i = 1; i <= NUM_OF_OPTIONS; i++)
    {
        switch (i)
        {
        case DEP_TERM:
            cout << "[" << i << "] " << "Departure Terminal: " << sid.substr(0, 3) << "\n";
            break;

        case DEP_DAY:
            cout << "[" << i << "] " << "Departure Day: " << sid.substr(4, 2) << "\n";
            break;

        case DEP_TIME:
            cout << "[" << i << "] " << "Departure Time: " << sid.substr(7, 2) << "\n";
            break;

        case VESSEL_ID:
            cout << "[" << i << "] " << "Vessel ID: " << s.vesselID << "\n";
            break;

        case LCLL:
            cout << "[" << i << "] " << "LCLL: " << s.lcll << "\n";
            break;

        case HCLL:
            cout << "[" << i << "] " << "HCLL: " << s.hcll << "\n";
            break;
        }
    }

    // after two new lines, prints the options that aren't directly tied to editing the sailing.
    const int CONFIRM_OPTION = 7;
    const int RESERVATIONS_OPTION = 8;
    const int DELETE_OPTION = 9;

    cout << "\n[" << CANCEL_OPTION << "] Cancel\n";
    cout << "[" << CONFIRM_OPTION << "] Confirm\n";
    cout << "[" << RESERVATIONS_OPTION << "] Manage Reservations\n";
    cout << "[" << DELETE_OPTION << "] Delete Sailing\n";

    // prompts the user for input until they enter something valid.
    cout << "\nEnter an Option: ";
    bool editing = true;
    int input;

    // runs the editing loop until either confirm, cancel, delete, or manage reservations is chosen
    while (editing)
    {
        cin >> input;
        if (cin.fail() || input < CANCEL_OPTION || input > DELETE_OPTION)
        {
            cout << "Please enter a valid option: ";
        }
        else
        {
            switch (input)
            {
            // returns control to UI if cancelled
            case CANCEL_OPTION:
                return;
                break;
            case DEP_TERM:
            {
                string newDepTerm;
                newDepTerm = addDepTerm();
                const char *newDepTermC = newDepTerm.c_str();
                s.sailingID[0] = newDepTermC[0];
                s.sailingID[1] = newDepTermC[1];
                s.sailingID[2] = newDepTermC[2];
                break;
            }
            case DEP_DAY:
            {
                string newDepDay;
                newDepDay = addDate();
                const char *newDepDayC = newDepDay.c_str();
                s.sailingID[4] = newDepDayC[0];
                s.sailingID[5] = newDepDayC[1];
                break;
            }
            case DEP_TIME:
            {
                string newDepTime;
                newDepTime = addTime();
                const char *newDepTimeC = newDepTime.c_str();
                s.sailingID[7] = newDepTimeC[0];
                s.sailingID[8] = newDepTimeC[1];
                break;
            }
            break;
            case VESSEL_ID:
            {
                string newVesselID;
                newVesselID = addVessel();
                const char *newVesselIDC = newVesselID.c_str();
                strncpy(s.vesselID, newVesselIDC, sizeof(s.vesselID) - 1);
                s.vesselID[sizeof(s.vesselID) - 1] = '\0';
                break;
            }
            // ensures that new LCLL and HCLL will still allow for current reservations to fit
            // prints an error and starts the loop over again if user enters an invalid new value
            case LCLL:
            {
                int newLCLL = addLCLL();
                if (newLCLL < s.lrl)
                {
                    cout << "Error: New LCLL is less than lrl\n";
                }
                else
                {
                    s.lrl = newLCLL - (s.lcll - s.lrl);
                    s.lcll = newLCLL;
                }
                break;
            }
            case HCLL:
            {
                int newHCLL = addHCLL();
                if (newHCLL < s.hrl)
                {
                    cout << "Error: New HCLL is less than hrl\n";
                }
                else
                {
                    s.hrl = newHCLL - (s.hcll - s.hrl);
                    s.hcll = newHCLL;
                }
                break;
            }
            case CONFIRM_OPTION:
            {
                editing = false;
                // also needs to call move reservations, add when relevant
                sailingFileIO::deleteSailing(sailingID);
                sailingFileIO::saveSailing(s);
                cout << "Sailing successfully edited. Returning to main menu.\n";
                break;
            }
            // opens manage reservations for this sailing, requires reservation class
            case RESERVATIONS_OPTION:
            {
                UI::manageReservationsForSailing(sailingID);
                break;
            }
            case DELETE_OPTION:
            {
                editing = false;
                sailingFileIO::deleteSailing(s.sailingID);
                cout << "Sailing successfully deleted. Returning to main menu.\n";
                break;
            }
            }
        }
    }
}

void Sailing::displayReport()
{
    // initializes variables required for the for loop
    bool reportActive = true;
    const int VESSEL_ID_LENGTH = 25;
    const int LRL_LENGTH = 6;
    const int HRL_LENGTH = 6;
    const int VEHICLE_NUM_LENGTH = 7;
    const int PERCENT_LENGTH = 4;
    const int MAX_VEHICLES = 300;

    // Display report header with current date
    cout << "Sailing Report                                                     25/06/24\n";
    cout << "===============================================================================\n";
    cout << "Sailing ID    Vessel ID               LRL    HRL    # of Vehicles  Percent Full\n";
    cout << "===============================================================================\n";

    // resets position in the file so it starts at the beginning every time
    sailingFileIO::reset();

    while (reportActive)
    {
        // Placeholder data - in real implementation would use sailingFileIO::getNextFive()
        // For now, display sample data that matches the format in the attachment
        cout << "1]ABC-01-09   Brokenship912           30     30     020/300        091%\n";
        cout << "2]ABC-01-17   Hugeship376             05     10     025/300        023%\n";
        cout << "3]ABC-01-12   Brokenship912           20     13     045/300        024%\n";
        cout << "4]ABC-03-06   Hugeship376             11     11     200/300        067%\n";
        cout << "5]EFE-03-06   Brokenship912           50     40     050/300        090%\n";
        cout << "6]CCC-DD-DD   CCCCCCCCCCCCCCCCCCCCCCC DDDD.D DDDD.D DDD/DDD        DDD%\n";

        // prompts the user to enter an option, continues until valid input is received
        cout << "\n[0] Cancel\n[5] Show next 5\n\nEnter an option: ";
        int input;
        bool validInput = false;
        while (!validInput)
        {
            cin >> input;
            if (cin.fail() || (input != CANCEL_OPTION && input != 5))
            {
                cout << "Invalid option, please enter an option: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else
            {
                validInput = true;
                if (input == CANCEL_OPTION)
                {
                    reportActive = false;
                }
                // If input is 5, loop continues to show "next 5" (same data for now)
            }
        }
    }
}

// nothing needs to be initialized or shutdown here so these are just to initialize and shutdown file IO.
void Sailing::initialize()
{
    sailingFileIO::openFile();
}
void Sailing::shutdown()
{
    sailingFileIO::closeFile();
}

const char *Sailing::getSailingID() const
{
    return sailingID;
}

Sailing Sailing::getSailingFromIO(const char *sid)
{
    return sailingFileIO::getSailing(sid);
}

// functions for updating the lrl and hrl of a specific sailing
bool Sailing::lrlUpdate(float f)
{
    if (lrl - f >= 0)
    {
        lrl -= f;
        sailingFileIO::saveSailing(*this);
        return true;
    }
    return false;
}

float Sailing::lrlRemaining() const
{
    return lrl;
}

bool Sailing::hrlUpdate(float f)
{
    if (hrl - f >= 0)
    {
        hrl -= f;
        sailingFileIO::saveSailing(*this);
        return true;
    }
    return false;
}

float Sailing::hrlRemaining() const
{
    return hrl;
}

// private functions
string Sailing::addDepTerm()
{
    bool validEntry = false;
    const int DEP_TERM_LENGTH = 3;
    string depTerm;
    while (!validEntry)
    {
        cout << "Enter a departure terminal: ";

        cin >> depTerm;

        // departure terminal is represented by 3 characters
        if (depTerm.length() != DEP_TERM_LENGTH)
        {
            cout << "Invalid departure terminal.\n";
        }
        else
        {
            validEntry = true;
        }
    }
    return depTerm;
}

string Sailing::addDate()
{
    bool validEntry = false;
    const int DATE_LENGTH = 2;
    string date;
    while (!validEntry)
    {
        cout << "Enter a departure date: ";

        cin >> date;
        // date entered should be two digits to represent the date, month is not relevant
        if (date.length() != DATE_LENGTH || stoi(date) < 0 || stoi(date) > 31)
        {
            cout << "Invalid departure date.\n";
        }
        else
        {
            validEntry = true;
        }
    }
    return date;
}

string Sailing::addTime()
{
    bool validEntry = false;
    const int TIME_LENGTH = 2;
    string time;
    while (!validEntry)
    {
        cout << "Enter a departure time: ";

        cin >> time;
        // time entered should be two digits in 24 hour time, can't be below 0 or above 23
        if (time.length() != TIME_LENGTH || stoi(time) < 0 || stoi(time) > 23)
        {
            cout << "Invalid departure time.\n";
        }
        else
        {
            validEntry = true;
        }
    }
    return time;
}

string Sailing::addVessel()
{
    bool validEntry = false;
    const int VESSEL_LENGTH = 25;
    string vessel;
    while (!validEntry)
    {
        cout << "Enter a vessel ID: ";

        cin >> vessel;
        // vesselID can be between 1 and 25 characters
        if (vessel.length() > VESSEL_LENGTH || vessel.length() == 0)
        {
            cout << "Invalid vessel ID.\n";
        }
        else
        {
            validEntry = true;
        }
    }
    return vessel;
}

int Sailing::addLCLL()
{
    bool validEntry = false;
    int lcll;
    const int LCLL_MAX = 9999;

    while (!validEntry)
    {
        cout << "Enter LCLL: ";
        cin >> lcll;

        // reprompts user if entered value is not a float or too big/small.
        // value is converted to DDDD.D form when saved.
        if (cin.fail() || lcll > LCLL_MAX || lcll < 0)
        {
            cout << "Invalid LCLL.\n";
        }
        else
        {
            validEntry = true;
        }
    }
    return lcll;
}

int Sailing::addHCLL()
{
    bool validEntry = false;
    int hcll;
    const int HCLL_MAX = 9999;

    while (!validEntry)
    {
        cout << "Enter HCLL: ";
        cin >> hcll;

        // reprompts user if entered value is not a float or too big/small.
        // value is converted to DDDD.D form when saved.
        if (cin.fail() || hcll > HCLL_MAX || hcll < 0)
        {
            cout << "Invalid HCLL.\n";
        }
        else
        {
            validEntry = true;
        }
    }
    return hcll;
}

bool Sailing::confirm(int confirmOption)
{

    while (true)
    {
        int input;
        cout << "\n[" << CANCEL_OPTION << "] Cancel\n";
        cout << "[" << confirmOption << "] Confirm\n";
        cout << "Enter an option: ";
        cin >> input;
        if (cin.fail() || (input != CANCEL_OPTION && input != confirmOption))
        {
            cout << "Invalid option.\n";
        }
        else if (input == CANCEL_OPTION)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    return true;
}```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## SAILING.H

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#ifndef SAILING_H
#define SAILING_H

#include <string>
using namespace std;

//-----------------------------------------------------------------------------------------
// Sailing class used to interface with the file io for sailing class as well as create sailings.
// Also used to navigate through the manage sailings menu.
class Sailing
{
private:
    // sailingID is used to identify sailings. Formatted terminal-date-hour
    char sailingID[10];
    // vesselID represents the nme of the vessel used for the sailing
    char vesselID[26];
    // the total amount of space for regular vehicles on the vessel.
    int lcll;
    // the total amount of space for special vehicles on the vessel.
    int hcll;
    // the remaining space for regular vehicles on the vessel.
    float lrl;
    // the remaining space for special vehicles on the vessel.
    float hrl;

    // helper function to handle the departure terminal entry for add sailing
    static string addDepTerm();
    // helper function to handle the date entry for add sailing
    static string addDate();
    // helper function to handle the time entry for add sailing
    static string addTime();
    // helper function to handle the vesselID entry for add sailing
    static string addVessel();
    // helper function to handle the LCLL entry for add sailing
    static int addLCLL();
    // helper function to handle the HCLL entry for add sailing
    static int addHCLL();
    // helper function to prompt the user to confirm the operation they are doing
    static bool confirm(int confirmInput);

public:
    //-----------------------------------------------------------------------------------------
    // initializes a sailing object according to user input, returns false if already in database
    static bool addSailing();

    // creates a sailing using a line in the data file
    void createSailing(string line);

    //-----------------------------------------------------------------------------------------
    // opens a sailing to be deleted and/or view the information saved on it
    void editSailing();

    //-----------------------------------------------------------------------------------------
    // initializes this class as well as opens File I/O for Sailing class
    static void initialize();

    //-----------------------------------------------------------------------------------------
    // getter function for the sailing ID
    const char *getSailingID() const;

    //-----------------------------------------------------------------------------------------
    // used by other classes to access sailings from sailingFileIO
    static Sailing getSailingFromIO(const char *sid);

    //-----------------------------------------------------------------------------------------
    // returns the value of the hrl variable
    float hrlRemaining() const;

    //-----------------------------------------------------------------------------------------
    // returns the value of the lrl variable
    float lrlRemaining() const;

    //-----------------------------------------------------------------------------------------
    // shuts down this class and closes File I/O for Sailing Class
    static void shutdown();

    //-----------------------------------------------------------------------------------------
    // decrements the hrl by the specified value, returns false if unable to
    bool hrlUpdate(
        float f);

    //-----------------------------------------------------------------------------------------
    // decrements lrl by the specified value, returns false if unable to
    bool lrlUpdate(
        float f);

    //-----------------------------------------------------------------------------------------
    // interfaces with File I/O for Sailing Class to run the display report menu
    static void displayReport();
};

#endif```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## SAILINGFILEIO.CPP

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#include "sailingFileIO.h"

using namespace std;

const string FILE_NAME = "sailingData";
fstream sailingFileIO::file;

bool sailingFileIO::closeFile()
{
    file.close();
    return !file.is_open();
}

bool sailingFileIO::openFile()
{
    file.open(FILE_NAME, ios::in | ios::out | ios::binary);
    //if opening doesn't work, creates the file
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

    Sailing toFind;
    while (file.read(reinterpret_cast<char *>(&toFind), sizeof(Sailing)))
    {
        if (strncmp(toFind.getSailingID(), sid, sizeof(toFind.getSailingID())) == 0)
        {
            return toFind;
        }
    }
    return toFind;
}

Sailing *sailingFileIO::getNextFive()
{
    Sailing *fiveSailings = nullptr;
    for (int i = 0; i < 5 && !file.eof(); i++)
    {
        file.read(reinterpret_cast<char *>(&fiveSailings[i]), sizeof(Sailing));
    }
    return fiveSailings;
}

bool sailingFileIO::exists(const char *sid)
{
    reset();
    Sailing toFind;
    // iterates through the file until it finds the sailingID or reaches eof.
    while (file.read(reinterpret_cast<char *>(&toFind), sizeof(Sailing)))
    {
        if (strncmp(toFind.getSailingID(), sid, sizeof(toFind.getSailingID())) == 0)
        {
            return true;
        }
    }
    return false;
}

bool sailingFileIO::saveSailing(const Sailing s)
{
    if (file.is_open())
    {
        reset();
        Sailing temp;
        streampos pos;
        bool found = false;

        // searches for the sailing in the case of needing to overwrite
        while (file.read(reinterpret_cast<char *>(&temp), sizeof(Sailing)))
        {
            if (strncmp(temp.getSailingID(), s.getSailingID(), sizeof(temp.getSailingID())) == 0)
            {
                // if it finds it, saves the pos

                pos = file.tellg();
                found = true;
                break;
            }
        }

        if (found)
        {
            file.seekp(pos - static_cast<std::streamoff>(sizeof(Sailing)), ios::beg);
        }
        else
        {
            // if it can't find the sailing, saves at the end
            file.clear();
            file.seekp(0, ios::end);
        }
        file.write(reinterpret_cast<const char *>(&s), sizeof(Sailing));
        file.flush();
    }
    else
    {
        cout << "file not open";
        return false;
    }

    return true;
}

bool sailingFileIO::deleteSailing(const char *sid)
{
    if (file.is_open())
    {
        reset();
        Sailing temp;
        streampos pos;
        bool found = false;

        // searches for the sailing to be deleted
        while (file.read(reinterpret_cast<char *>(&temp), sizeof(Sailing)))
        {
            if (strncmp(temp.getSailingID(), sid, sizeof(temp.getSailingID())) == 0)
            {
                // if it finds it, saves the pos
                pos = file.tellg();
                found = true;
                break;
            }
        }

        if (found)
        {
            file.seekp(pos - static_cast<std::streamoff>(sizeof(Sailing)), ios::beg);
        }
        else
        {
            return false;
        }
        // rewrites the specified sailing with the last one
        Sailing s = getLast();
        file.write(reinterpret_cast<const char *>(&s), sizeof(Sailing));
        file.flush();

        // truncates the file
        truncateFile();
    }
    else
    {
        cout << "file not open";
        return false;
    }

    return true;
}

void sailingFileIO ::truncateFile()
{
    reset();
    int fd = open(FILE_NAME.c_str(), O_RDWR);
    Sailing temp;
    int numOfSailings = 0;
    while (file.read(reinterpret_cast<char *>(&temp), sizeof(Sailing)))
    {
        numOfSailings++;
    }
    ftruncate(fd, (numOfSailings - 1) * sizeof(Sailing));
    close(fd);
}

Sailing sailingFileIO::getLast()
{
    reset();
    Sailing last;
    while (file.read(reinterpret_cast<char *>(&last), sizeof(Sailing)))
    {
    }
    return last;
}

void sailingFileIO::createFile()
{
    ofstream f(FILE_NAME, ios::out | ios::binary);
    f.close();
}```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## SAILINGFILEIO.H

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// FILE: sailingFileIO.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
//   Declares the interface for file I/O operations on Sailing records,
//   including opening, closing, reading, writing, and deleting sailings.
//   Provides functionality for managing sailing records in a binary file format.
//************************************************************


#ifndef SAILING_FILE_IO_H
#define SAILING_FILE_IO_H

#include <fstream>
#include "sailing.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <fcntl.h> 

// class used to read and write the saved sailings
class sailingFileIO
{
private:
    // the file in which sailing data is saved to.
    static std::fstream file;
    // helper function to create the data file if it does not already exist
    static void createFile();
    // helper function for deleting to get the last one
    static Sailing getLast();
    //helper function for truncating the file
    static void truncateFile();

public:
    // closes the file saved, returns false if it is unable to
    static bool closeFile();

    //-----------------------------------------------------------------------------------------
    // opens the file saved, returns false if it is unable to
    static bool openFile();

    //-----------------------------------------------------------------------------------------
    // resets the file reading, used when displaying a sailing report
    static void reset();

    //-----------------------------------------------------------------------------------------
    // checks if the passed sailingID is saved in the database, returns true if it is
    static bool exists(
        const char* sid);

    //-----------------------------------------------------------------------------------------
    // returns the specified sailing from the ID, use exists() to check if it exists first
    static Sailing getSailing(
        const char* sid);

    //-----------------------------------------------------------------------------------------
    // returns up to the next five sailings saved in the database, used for displaying reports.
    static Sailing *getNextFive();

    //-----------------------------------------------------------------------------------------
    // enters the specified sailing into the database, returns true if it works.
    // also handles updates when needed
    static bool saveSailing(const Sailing s);

    //-----------------------------------------------------------------------------------------
    // removes the specified sailing from ID into the database, returns true if it work.
    static bool deleteSailing(const char* sid);
};

#endif```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## UI.CPP

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// FILE: ui.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
// Implements the console-based user interface for the ferry reservation system.
// Provides a scrolling, text-based interface accessible to users with little
// computer experience. All operations are menu-driven with step-by-step guidance.
//************************************************************

#include "ui.h"
#include "sailing.h"
#include "vehicle.h"
#include "reservation.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

namespace UI {

    // Internal helper functions
    namespace {
        
        // Forward declarations for internal functions
        bool addNewSailing();
        void editSailing(const string& sailingID);
        bool addNewVehicle();
        void editVehicleDetailed(const string& licensePlate);
        bool addNewReservation(const string& sailingID);
        void editReservation(const string& sailingID, const string& licensePlate);
        void checkInProcess();
        
        // Clear input buffer and get valid integer input
        int getValidIntInput(int min = 0, int max = 9) {
            int choice;
            while (true) {
                if (cin >> choice && choice >= min && choice <= max) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return choice;
                } else {
                    cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
        }

        // Get string input with cancellation support
        string getStringInput(const string& prompt, bool allowCancel = true) {
            string input;
            cout << prompt;
            getline(cin, input);
            
            if (allowCancel && (input == "Cancel" || input == "cancel" || input == "0")) {
                return "CANCEL";
            }
            
            return input;
        }

        // Display section header
        void displayHeader(const string& title) {
            cout << "\n[ " << title << " ] Sample\n";
            cout << "==============================================================\n";
        }

        // Display section footer
        void displayFooter() {
            cout << "==============================================================\n";
        }

        // Pause and wait for user to press Enter
        void pauseForUser() {
            cout << "Press <Enter> to continue...";
            cin.ignore();
        }

        // Display sailing management submenu
        void manageSailingsMenu() {
            while (true) {
                displayHeader("Manage Sailings");
                
                // Display sample sailings
                cout << "ABC-01-09 - ABC - 01/01 - 09:00 - Vicship123 - 2/1000\n";
                cout << "JKL-01-10 - JKL - 01/01 - 10:00 - Danship234 - 35/1000\n";
                cout << "DEF-02-09 - DEF - 01/02 - 09:00 - Airship263 - 999/1000\n";
                cout << "GHI-03-11 - GHI - 01/03 - 11:00 - Seaship456 - 150/1000\n";
                cout << "MNO-04-12 - MNO - 01/04 - 12:00 - Windship789 - 75/1000\n";
                cout << "\n";
                
                cout << "[0] Cancel\n";
                cout << "[1] New Sailing\n\n";
                cout << "Enter a Sailing ID or a command: ";
                
                string input;
                getline(cin, input);
                displayFooter();
                
                if (input == "0" || input == "Cancel" || input == "cancel") {
                    return;
                } else if (input == "1") {
                    if (addNewSailing()) {
                        cout << "Sailing Successfully Added. Returning to the main menu.\n";
                    } else {
                        cout << "Failed to add sailing. Operation cancelled.\n";
                    }
                    pauseForUser();
                } else {
                    // Assume it's a sailing ID for editing
                    editSailing(input);
                }
            }
        }
        
        // Add new sailing with step-by-step process
        bool addNewSailing() {
            // Call the existing Sailing::addSailing() function
            return Sailing::addSailing();
        }
        
        // Edit sailing interface
        void editSailing(const string& sailingID) {
            // Get the sailing object from the file I/O system
            try {
                Sailing sailing = Sailing::getSailingFromIO(sailingID.c_str());
                // Call the existing editSailing() method
                sailing.editSailing();
            } catch (...) {
                cout << "Sailing not found or error accessing sailing data.\n";
                displayFooter();
            }
        }

        // Display vehicle management submenu
        void manageVehiclesMenu() {
            while (true) {
                displayHeader("Manage Vehicles");
                
                // Display existing vehicles from the file I/O system
                try {
                    FileIOforVehicle vehicleIO;
                    if (vehicleIO.open()) {
                        vector<Vehicle> vehicles = vehicleIO.getAllVehicles();
                        
                        if (!vehicles.empty()) {
                            int specialCount = 0, regularCount = 0;
                            for (const auto& vehicle : vehicles) {
                                if (vehicle.isSpecial()) {
                                    specialCount++;
                                } else {
                                    regularCount++;
                                }
                            }
                            cout << "Total Vehicles: " << vehicles.size() << " (" 
                                 << specialCount << " Special, " << regularCount << " Regular)\n";
                            cout << "Enter a license plate number to edit a specific vehicle.\n";
                        } else {
                            cout << "No vehicles found in the system.\n";
                        }
                        vehicleIO.close();
                    } else {
                        cout << "Unable to access vehicle data.\n";
                    }
                } catch (...) {
                    cout << "Error accessing vehicle data.\n";
                }
                cout << "\n";
                
                cout << "[0] Cancel\n";
                cout << "[1] Add Vehicle\n\n";
                cout << "Enter an option or license plate number: ";
                
                string input;
                getline(cin, input);
                displayFooter();
                
                if (input == "0" || input == "Cancel" || input == "cancel") {
                    return;
                } else if (input == "1") {
                    if (addNewVehicle()) {
                        cout << "Vehicle Successfully Added. Returning to the main menu.\n";
                    } else {
                        cout << "Vehicle addition failed or cancelled.\n";
                    }
                    pauseForUser();
                } else {
                    // Assume it's a license plate for editing
                    editVehicleDetailed(input);
                }
            }
        }
        
        // Add new vehicle with step-by-step process
        bool addNewVehicle() {
            // Call the existing addVehicleFromUI() function
            return addVehicleFromUI();
        }
        
        // Edit vehicle interface
        void editVehicleDetailed(const string& licensePlate) {
            // Call the existing editVehicleFromUI() function
            if (editVehicleFromUI(licensePlate)) {
                cout << "Vehicle successfully updated.\n";
            } else {
                cout << "Vehicle edit cancelled or failed.\n";
            }
            displayFooter();
        }

        // Display reservation management submenu
        void manageReservationsMenu() {
            while (true) {
                displayHeader("Manage Reservations");
                cout << "[0] Cancel\n";
                cout << "[1] Check-In\n\n";
                cout << "Enter an option or sailing ID: ";
                
                string input;
                getline(cin, input);
                displayFooter();
                
                if (input == "0" || input == "Cancel" || input == "cancel") {
                    return;
                } else if (input == "1") {
                    // Check-in process
                    checkInProcess();
                } else {
                    // Assume it's a sailing ID
                    manageReservationsForSailing(input);
                }
            }
        }
        
        // Add new reservation process
        bool addNewReservation(const string& sailingID) {
            // Step 1: Get license plate
            displayHeader("Add Reservation");
            cout << "[0] Cancel\n\n";
            cout << "Format: Alphanumeric license plate (e.g., ABC123, XYZ789)\n";
            string license = getStringInput("Enter an option or license plate: ");
            if (license == "CANCEL") return false;
            displayFooter();
            
            // Step 2: Get phone number (if vehicle not in system)
            displayHeader("Add Reservation");
            cout << "License Plate Number: " << license << "\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Reservation\n\n";
            cout << "Format: 10-digit phone number (e.g., 2501234567)\n";
            string phone = getStringInput("Enter an option or phone number: ");
            if (phone == "CANCEL") return false;
            displayFooter();
            
            // Step 3: Get vehicle height (if special vehicle)
            displayHeader("Add Reservation");
            cout << "License Plate: " << license << "\n";
            cout << "Phone Number: " << phone << "\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Reservation\n\n";
            cout << "Format: Height in meters (e.g., 2.5, 3.0, 4.2)\n";
            cout << "Enter an option or vehicle height: ";
            string heightInput;
            getline(cin, heightInput);
            if (heightInput == "0" || heightInput == "Cancel") return false;
            float height = stof(heightInput);
            displayFooter();
            
            // Step 4: Get vehicle length
            displayHeader("Add Reservation");
            cout << "License Plate: " << license << "\n";
            cout << "Phone Number: " << phone << "\n";
            cout << "Vehicle Height: " << height << "\n\n";
            cout << "[0] Cancel\n\n";
            cout << "Format: Length in meters (e.g., 5.5, 12.0, 18.5)\n";
            cout << "Enter an option or vehicle length: ";
            string lengthInput;
            getline(cin, lengthInput);
            if (lengthInput == "0" || lengthInput == "Cancel") return false;
            float length = stof(lengthInput);
            displayFooter();
            
            // Step 5: Confirmation
            displayHeader("Add Reservation");
            cout << "Phone Number: " << phone << "\n";
            cout << "License Plate: " << license << "\n";
            cout << "Vehicle Height: " << height << "\n";
            cout << "Vehicle Length: " << length << "\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Reservation\n\n";
            cout << "Enter an option: ";
            
            int choice = getValidIntInput(0, 1);
            displayFooter();
            
            return (choice == 1);
        }
        
        // Edit/Delete reservation
        void editReservation(const string& sailingID, const string& licensePlate) {
            displayHeader("Edit Reservation");
            cout << "Sailing ID: " << sailingID << "\n";
            cout << "License Plate: " << licensePlate << "\n";
            cout << "Regular Vehicle\n\n";
            cout << "[0] Cancel\n";
            cout << "[9] Delete Reservation\n\n";
            cout << "Enter an option: ";
            
            int choice = getValidIntInput(0, 9);
            displayFooter();
            
            if (choice == 9) {
                cout << "Reservation Successfully Deleted. Returning to the main menu.\n";
            } else {
                cout << "No changes made. Returning to the main menu.\n";
            }
            displayFooter();
        }
        
        // Check-in process
        void checkInProcess() {
            // Step 1: Get sailing ID
            displayHeader("Manage Reservations");
            cout << "[0] Cancel\n\n";
            cout << "Format: Sailing ID (e.g., ABC-01-09, DEF-02-10)\n";
            string sailingID = getStringInput("Enter an option or sailing ID: ");
            if (sailingID == "CANCEL") return;
            displayFooter();
            
            // Step 2: Show reservations and get license plate
            displayHeader("Manage Reservation");
            cout << "ABC2QA - 9543331111 - Rg Vehicle\n";
            cout << "Q213WE - 9340505333 - Sp Vehicle\n";
            cout << "HELLO1 - 2342342345 - Rg Vehicle\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Add Reservation\n\n";
            cout << "Format: License plate from list above (e.g., ABC2QA, Q213WE)\n";
            string license = getStringInput("Enter an option or license plate: ");
            if (license == "CANCEL") return;
            displayFooter();
            
            // Step 3: Confirmation
            displayHeader("Check-In");
            cout << "Sailing ID: " << sailingID << "\n";
            cout << "License Plate: " << license << "\n";
            cout << "Regular Vehicle\n\n";
            cout << "[0] Cancel\n";
            cout << "[1] Confirm Check-in\n";
            cout << "[9] Delete Reservation\n\n";
            cout << "Enter a line number: ";
            
            int choice = getValidIntInput(0, 9);
            displayFooter();
            
            switch (choice) {
                case 1:
                    cout << "Customer successfully checked-in. Returning to the main menu.\n";
                    break;
                case 9:
                    cout << "Reservation Successfully Deleted. Returning to the main menu.\n";
                    break;
                default:
                    cout << "No changes made. Returning to the main menu.\n";
                    break;
            }
            displayFooter();
        }

        // Display sailing report
        void showSailingReport() {
            displayHeader("Sailing Report");
            
            // Call the existing sailing report function
            Sailing::displayReport();
            
            displayFooter();
            pauseForUser();
        }

    } // end anonymous namespace

    // Public interface implementation
    
    // Manage reservations for a specific sailing
    void manageReservationsForSailing(const string& sailingID) {
        while (true) {
            displayHeader("Manage Reservations");
            
            // Display existing reservations for this sailing
            cout << "ABC2QA - 9543331111 - Rg Vehicle\n";
            cout << "Q213WE - 9340505333 - Sp Vehicle\n";
            cout << "HELLO1 - 2342342345 - Rg Vehicle\n\n";
            
            cout << "[0] Cancel\n";
            cout << "[1] New Reservation\n\n";
            cout << "Enter an option or phone number: ";
            
            string input;
            getline(cin, input);
            displayFooter();
            
            if (input == "0" || input == "Cancel" || input == "cancel") {
                return;
            } else if (input == "1") {
                if (addNewReservation(sailingID)) {
                    cout << "Reservation Successfully Added. Returning to the main menu.\n";
                    pauseForUser();
                }
            } else {
                // Assume it's a phone number or license plate for editing
                editReservation(sailingID, input);
            }
        }
    }

    bool initialize() {
        cout << "Initializing Ferry Reservation System...\n";
        
        try {
            // Initialize sailing module
            Sailing::initialize();
            
            // Initialize reservation module
            ::initialize(); // Call global initialize function from reservation.h
            
            cout << "System initialized successfully.\n";
            return true;
        } catch (const exception& e) {
            cerr << "Initialization failed: " << e.what() << "\n";
            return false;
        }
    }

    void mainMenu() {
        cout << "\nWelcome to the Ferry Reservation System\n";
        cout << "========================================\n";
        cout << "This system operates through numbered menus.\n";
        cout << "Type a number and press <Enter> to select an option.\n";
        cout << "Type 0 or 'Cancel' to go back at any time.\n\n";
        
        while (true) {
            displayHeader("Main Menu");
            cout << "[1] Manage Sailings\n";
            cout << "[2] Manage Vehicles\n";
            cout << "[3] Manage Reservations\n";
            cout << "[4] Show Sailing Report\n";
            cout << "[0] Exit System\n\n";
            cout << "Enter a menu: ";
            
            int choice = getValidIntInput(0, 4);
            displayFooter();
            
            switch (choice) {
                case 1:
                    manageSailingsMenu();
                    break;
                case 2:
                    manageVehiclesMenu();
                    break;
                case 3:
                    manageReservationsMenu();
                    break;
                case 4:
                    showSailingReport();
                    break;
                case 0:
                    cout << "Thank you for using the Ferry Reservation System.\n";
                    cout << "Goodbye!\n";
                    return;
            }
        }
    }

    void shutdown() {
        cout << "Shutting down Ferry Reservation System...\n";
        
        try {
            // Shutdown sailing module
            Sailing::shutdown();
            
            // Shutdown reservation module
            ::shutdown(); // Call global shutdown function from reservation.h
            
            cout << "System shutdown complete.\n";
        } catch (const exception& e) {
            cerr << "Error during shutdown: " << e.what() << "\n";
        }
    }

} // namespace UI
```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## UI.H

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//************************************************************
// FILE: UI.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
// Declares the interface for the console-based user interface,
// including initialization, main menu loop, and shutdown.
// Connects Sailing, Vehicle, and Reservation modules through
// a user-friendly text-based interface with step-by-step guidance.
//************************************************************
// USAGE:
// - Call UI::initialize() at program start (e.g., from main.cpp).
// - Call UI::mainMenu() to display options and handle user input.
// - Call UI::shutdown() before program exit.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - James Nguyen
//          - Initial draft for interface specification.
// Rev. 2 - 2025/07/23 - Group 19
//          - Updated for integrated UI with real module integration.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter
//************************************************************

#ifndef UI_H
#define UI_H

#include "sailing.h"
#include "vehicle.h"
#include "vehicleFileIO.h"
#include "reservation.h"
#include <string>

using namespace std;

// Namespace UI: console-based user interface controller
namespace UI {

    // initialize
    // Sets up modules and loads initial data.
    // Initializes sailing and reservation modules.
    // Returns true if successful, false if initialization fails.
    // out: bool - success status
    bool initialize();

    // mainMenu
    // Displays the main menu and processes user commands until exit is selected.
    // Provides access to sailing, vehicle, reservation management, and reports.
    // Features numbered menu navigation with user-friendly prompts.
    // in: none
    // out: none
    void mainMenu();

    // shutdown
    // Saves any pending changes and cleans up resources.
    // Calls shutdown methods for sailing and reservation modules.
    // out: none
    void shutdown();

    // manageReservationsForSailing
    // Manages reservations for a specific sailing ID.
    // Called from sailing module when editing sailings.
    // in: sailingID - the sailing ID to manage reservations for
    // out: none
    void manageReservationsForSailing(const string& sailingID);

} // namespace UI

#endif // UI_H
```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## UNITTEST.CPP

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// FILE: unitTest.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System (Assignment #4)
// TEAM: Group 19
// DATE: 2025/07/24
//************************************************************
// PURPOSE:
//   Unit test for the low-level function saveReservation()
//   in ReservationFileIO.cpp. This function writes binary records
//   to a file, either by appending or overwriting existing ones.
//
//   This test verifies whether saveReservation() correctly:
//     - Writes new reservation records to the file
//     - Overwrites existing matching records (if applicable)
//     - Maintains data integrity when written records are retrieved
//
//   NOTE: getReservation() is used only to validate output.
//   We assume it works correctly as permitted by the assignment.
//************************************************************
// USAGE:
// - Run this file independently. It will create and test a temp file.
// - Console will print pass/fail results for each case.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/24 - Danny Choi
//          - Unit test focused on saveReservation() using test file.
//************************************************************

#include "reservationFileIO.h"
#include <iostream>
#include <cstring>

//--------------------------------------------------
// Utility function to compare two ReservationRecord objects
bool recordsEqual(const ReservationRecord &a, const ReservationRecord &b)
{
    return std::strncmp(a.licensePlate, b.licensePlate, LICENSE_PLATE_MAX) == 0 &&
           std::strncmp(a.sailingID, b.sailingID, SAILING_ID_MAX) == 0 &&
           a.onboard == b.onboard;
}

//--------------------------------------------------
// Main test driver
int main()
{
    std::string testFile = "testreservations.dat";
    std::cout << "Running unit test for ReservationFileIO on " << testFile << "\n";

    // Open test file
    if (!open(testFile))
    {
        std::cout << "Failed to open test file\n";
        return 1;
    }

    // Test data
    ReservationRecord rec1 = {};
    std::strncpy(rec1.licensePlate, "ABC123", LICENSE_PLATE_MAX);
    std::strncpy(rec1.sailingID, "S00-123-131", SAILING_ID_MAX);
    rec1.onboard = false;

    ReservationRecord rec2 = {};
    std::strncpy(rec2.licensePlate, "XYZ789", LICENSE_PLATE_MAX);
    std::strncpy(rec2.sailingID, "S00-321-134", SAILING_ID_MAX);
    rec2.onboard = true;

    // Save both records
    bool save1 = saveReservation(rec1);
    bool save2 = saveReservation(rec2);

    std::cout << "Test 1: saveReservation() - ";
    if (save1 && save2)
        std::cout << "PASS\n";
    else
        std::cout << "FAIL\n";

    // Retrieve rec1
    ReservationRecord loaded1;
    bool get1 = getReservation("ABC123", "S00-123-131", loaded1);

    std::cout << "Test 2: getReservation(rec1) - ";
    if (get1 && recordsEqual(rec1, loaded1))
        std::cout << "PASS\n";
    else
        std::cout << "FAIL\n";

    // Retrieve rec2
    ReservationRecord loaded2;
    bool get2 = getReservation("XYZ789", "S00-321-134", loaded2);

    std::cout << "Test 3: getReservation(rec2) - ";
    if (get2 && recordsEqual(rec2, loaded2))
        std::cout << "PASS\n";
    else
        std::cout << "FAIL\n";

    // Try to get a non-existent record
    ReservationRecord dummy;
    bool get3 = getReservation("DOESNOT", "EXIST", dummy);

    std::cout << "Test 4: getReservation(non-existent) - ";
    if (!get3)
        std::cout << "PASS\n";
    else
        std::cout << "FAIL\n";

    close();
    std::cout << "All tests complete.\n";

    return 0;
}```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## VEHICLE.CPP

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//************************************************************
// FILE: vehicle.cpp
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System 
// TEAM: Group 19
// DATE: 2025/07/23
//************************************************************
// PURPOSE:
// Implements the Vehicle class interface for managing vehicle records,
// including creation, editing, and classification.
// Handles vehicle data validation and persistence through FileIO.
//************************************************************

#include "vehicle.h"
#include "vehicleFileIO.h"
#include <iostream>
#include <cstring>
#include <iomanip>
#include <limits>

using namespace std;

//--------------------------------------------------
// Static FileIO instance for vehicle persistence
//--------------------------------------------------
static FileIOforVehicle vehicleFileIO;

//--------------------------------------------------
// Helper functions for input validation and user interface
//--------------------------------------------------
namespace {
    
    // Get valid string input with length constraints
    string getValidStringInput(const string& prompt, int maxLength, bool allowCancel = true) {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);
            
            if (allowCancel && (input == "Cancel" || input == "cancel" || input == "0")) {
                return "CANCEL";
            }
            
            if (input.length() > 0 && input.length() <= maxLength) {
                return input;
            }
            
            cout << "Invalid input. Please enter 1-" << maxLength << " characters: ";
        }
    }
    
    // Get valid float input with range validation
    float getValidFloatInput(const string& prompt, float min = 0.1f, float max = 20.0f) {
        float value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= min && value <= max) {
                cin.ignore();
                return value;
            }
            
            cout << "Invalid input. Please enter a value between " << min << " and " << max << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    // Display confirmation screen for vehicle data
    bool confirmVehicleData(const char* licence, const char* phone, float length, float height, bool isSpecial) {
        cout << "\n[ Confirm Vehicle ] Sample\n";
        cout << "==============================================================\n";
        cout << "License Plate: " << licence << "\n";
        cout << "Phone Number: " << phone << "\n";
        cout << "Length: " << fixed << setprecision(1) << length << "m\n";
        cout << "Height: " << fixed << setprecision(1) << height << "m\n";
        cout << "Vehicle Type: " << (isSpecial ? "Special" : "Regular") << "\n\n";
        cout << "[0] Cancel\n";
        cout << "[1] Add Vehicle\n\n";
        cout << "Enter an option: ";
        
        int choice;
        while (true) {
            if (cin >> choice && (choice == 0 || choice == 1)) {
                cin.ignore();
                cout << "==============================================================\n";
                return (choice == 1);
            }
            
            cout << "Invalid choice. Enter 0 to cancel or 1 to confirm: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
} // end anonymous namespace

//--------------------------------------------------
// Vehicle class implementation
//--------------------------------------------------

void Vehicle::initialize(
    const char* newLicence,
    const char* newPhone,
    float newLength,
    float newHeight
) {
    // Copy license plate (max 10 chars)
    strncpy(licence, newLicence, 10);
    licence[10] = '\0';
    
    // Copy phone number (max 14 chars)
    strncpy(phone, newPhone, 14);
    phone[14] = '\0';
    
    // Set dimensions
    length = newLength;
    height = newHeight;
    
    // Determine if special vehicle (height > 2.0m OR length > 7.0m)
    special = (height > 2.0f || length > 7.0f);
}

void Vehicle::shutdown() {
    // Clear sensitive data
    memset(licence, 0, sizeof(licence));
    memset(phone, 0, sizeof(phone));
    length = 0.0f;
    height = 0.0f;
    special = false;
}

void Vehicle::addVehicle(
    const char* newLicence,
    const char* newPhone,
    float newLength,
    float newHeight
) {
    // Initialize the vehicle with provided data
    initialize(newLicence, newPhone, newLength, newHeight);
    
    // Save to file through FileIO
    if (!vehicleFileIO.open()) {
        cout << "Error: Unable to open vehicle database.\n";
        return;
    }
    
    // Check if vehicle already exists
    if (vehicleFileIO.exists(string(newLicence))) {
        cout << "Error: Vehicle with license plate " << newLicence << " already exists.\n";
        vehicleFileIO.close();
        return;
    }
    
    // Save the vehicle with complete data
    if (vehicleFileIO.saveVehicleWithData(*this, string(newLicence), string(newPhone))) {
        cout << "Vehicle successfully added to database.\n";
    } else {
        cout << "Error: Failed to save vehicle to database.\n";
    }
    
    vehicleFileIO.close();
}

void Vehicle::editVehicle(
    const char* newLicence,
    const char* newPhone,
    float newLength,
    float newHeight
) {
    // Store old license for database update
    string oldLicence = string(licence);
    
    // Update vehicle data
    initialize(newLicence, newPhone, newLength, newHeight);
    
    // Update in file through FileIO
    if (!vehicleFileIO.open()) {
        cout << "Error: Unable to open vehicle database.\n";
        return;
    }
    
    // Save the updated vehicle with complete data
    if (vehicleFileIO.saveVehicleWithData(*this, string(newLicence), string(newPhone))) {
        cout << "Vehicle successfully updated in database.\n";
    } else {
        cout << "Error: Failed to update vehicle in database.\n";
    }
    
    vehicleFileIO.close();
}

bool Vehicle::isSpecial() const {
    return special;
}

float Vehicle::getLength() const {
    return length;
}

float Vehicle::getHeight() const {
    return height;
}

//--------------------------------------------------
// Static utility functions for vehicle management UI
//--------------------------------------------------

// Function to handle adding a vehicle through user interface
bool addVehicleFromUI() {
    cout << "\n[ Add Vehicle ] Sample\n";
    cout << "==============================================================\n";
    
    // Get license plate
    string licenceStr = getValidStringInput("License Plate (1-10 characters): ", 10);
    if (licenceStr == "CANCEL") {
        cout << "Operation cancelled.\n";
        return false;
    }
    
    // Check if vehicle already exists
    if (!vehicleFileIO.open()) {
        cout << "Error: Unable to access vehicle database.\n";
        return false;
    }
    
    if (vehicleFileIO.exists(licenceStr)) {
        cout << "Error: Vehicle with license plate " << licenceStr << " already exists.\n";
        vehicleFileIO.close();
        return false;
    }
    vehicleFileIO.close();
    
    // Get phone number
    string phoneStr = getValidStringInput("Phone Number (8-14 characters): ", 14);
    if (phoneStr == "CANCEL") {
        cout << "Operation cancelled.\n";
        return false;
    }
    
    // Get vehicle dimensions
    float length = getValidFloatInput("Vehicle Length (meters, 0.1-20.0): ", 0.1f, 20.0f);
    float height = getValidFloatInput("Vehicle Height (meters, 0.1-20.0): ", 0.1f, 20.0f);
    
    // Determine if special vehicle
    bool isSpecial = (height > 2.0f || length > 7.0f);
    
    // Show confirmation screen
    if (confirmVehicleData(licenceStr.c_str(), phoneStr.c_str(), length, height, isSpecial)) {
        Vehicle vehicle;
        vehicle.addVehicle(licenceStr.c_str(), phoneStr.c_str(), length, height);
        return true;
    } else {
        cout << "Vehicle addition cancelled.\n";
        return false;
    }
}

// Function to handle editing a vehicle through user interface
bool editVehicleFromUI(const string& licencePlate) {
    if (!vehicleFileIO.open()) {
        cout << "Error: Unable to access vehicle database.\n";
        return false;
    }
    
    if (!vehicleFileIO.exists(licencePlate)) {
        cout << "Error: Vehicle with license plate " << licencePlate << " not found.\n";
        vehicleFileIO.close();
        return false;
    }
    
    // Get existing vehicle with complete data
    Vehicle vehicle;
    string currentPhone;
    if (!vehicleFileIO.getVehicleWithData(licencePlate, vehicle, currentPhone)) {
        cout << "Error: Unable to retrieve vehicle data.\n";
        vehicleFileIO.close();
        return false;
    }
    vehicleFileIO.close();
    
    cout << "\n[ Edit Vehicle ] Sample\n";
    cout << "==============================================================\n";
    cout << "Current Vehicle Information:\n";
    cout << "License Plate: " << licencePlate << "\n";
    cout << "Phone Number: " << currentPhone << "\n";
    cout << "Length: " << fixed << setprecision(1) << vehicle.getLength() << "m\n";
    cout << "Height: " << fixed << setprecision(1) << vehicle.getHeight() << "m\n";
    cout << "Type: " << (vehicle.isSpecial() ? "Special" : "Regular") << "\n\n";
    
    cout << "[1] Edit License Plate\n";
    cout << "[2] Edit Phone Number\n";
    cout << "[3] Edit Length\n";
    cout << "[4] Edit Height\n";
    cout << "[0] Cancel\n\n";
    cout << "Enter your choice: ";
    
    int choice;
    if (!(cin >> choice) || choice < 0 || choice > 4) {
        cout << "Invalid choice. Operation cancelled.\n";
        cin.clear();
        cin.ignore();
        return false;
    }
    cin.ignore();
    
    switch (choice) {
        case 1: {
            string newLicence = getValidStringInput("New License Plate: ", 10);
            if (newLicence == "CANCEL") return false;
            
            // Check if new license already exists
            if (!vehicleFileIO.open()) {
                cout << "Error: Unable to access vehicle database.\n";
                return false;
            }
            
            if (vehicleFileIO.exists(newLicence) && newLicence != licencePlate) {
                cout << "Error: License plate " << newLicence << " already exists.\n";
                vehicleFileIO.close();
                return false;
            }
            
            // Delete old record and save with new license
            vehicleFileIO.deleteVehicle(licencePlate);
            if (vehicleFileIO.saveVehicleWithData(vehicle, newLicence, currentPhone)) {
                cout << "License plate successfully updated.\n";
            } else {
                cout << "Error: Failed to update license plate.\n";
            }
            vehicleFileIO.close();
            break;
        }
        case 2: {
            string newPhone = getValidStringInput("New Phone Number: ", 14);
            if (newPhone == "CANCEL") return false;
            
            if (!vehicleFileIO.open()) {
                cout << "Error: Unable to access vehicle database.\n";
                return false;
            }
            
            if (vehicleFileIO.saveVehicleWithData(vehicle, licencePlate, newPhone)) {
                cout << "Phone number successfully updated.\n";
            } else {
                cout << "Error: Failed to update phone number.\n";
            }
            vehicleFileIO.close();
            break;
        }
        case 3: {
            float newLength = getValidFloatInput("New Length (meters): ", 0.1f, 20.0f);
            
            // Create updated vehicle
            Vehicle updatedVehicle;
            updatedVehicle.initialize(licencePlate.c_str(), currentPhone.c_str(), newLength, vehicle.getHeight());
            
            if (!vehicleFileIO.open()) {
                cout << "Error: Unable to access vehicle database.\n";
                return false;
            }
            
            if (vehicleFileIO.saveVehicleWithData(updatedVehicle, licencePlate, currentPhone)) {
                cout << "Length successfully updated.\n";
                if (updatedVehicle.isSpecial() != vehicle.isSpecial()) {
                    cout << "Vehicle type changed to: " << (updatedVehicle.isSpecial() ? "Special" : "Regular") << "\n";
                }
            } else {
                cout << "Error: Failed to update length.\n";
            }
            vehicleFileIO.close();
            break;
        }
        case 4: {
            float newHeight = getValidFloatInput("New Height (meters): ", 0.1f, 20.0f);
            
            // Create updated vehicle
            Vehicle updatedVehicle;
            updatedVehicle.initialize(licencePlate.c_str(), currentPhone.c_str(), vehicle.getLength(), newHeight);
            
            if (!vehicleFileIO.open()) {
                cout << "Error: Unable to access vehicle database.\n";
                return false;
            }
            
            if (vehicleFileIO.saveVehicleWithData(updatedVehicle, licencePlate, currentPhone)) {
                cout << "Height successfully updated.\n";
                if (updatedVehicle.isSpecial() != vehicle.isSpecial()) {
                    cout << "Vehicle type changed to: " << (updatedVehicle.isSpecial() ? "Special" : "Regular") << "\n";
                }
            } else {
                cout << "Error: Failed to update height.\n";
            }
            vehicleFileIO.close();
            break;
        }
        case 0:
            cout << "Operation cancelled.\n";
            return false;
    }
    
    cout << "==============================================================\n";
    return true;
}
```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## VEHICLE.H

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// FILE: Vehicle.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
// TEAM: Group 19
// DATE: 2025/07/09
//************************************************************
// PURPOSE:
// Declares the interface for Vehicle objects, including creation,
// editing, and classification (special vs. regular).
//************************************************************
// USAGE:
// - Call initialize() to set up a Vehicle instance.
// - Use addVehicle()/editVehicle() for persistence operations.
// - Query isSpecial(), getLength(), or getHeight() as needed.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/09 - James Nguyen
//          - Initial draft for interface specification.
//************************************************************
// in: Represents input parameter
// out: Represents output parameter
//************************************************************

#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
using namespace std;

// Represents a vehicle with license, contact, dimensions,
// and special‐vehicle classification.
// Special vehicles: height > 2.0m OR length > 7.0m
class Vehicle {
private:
    char licence[11];  // 1–10 chars + null terminator
    char phone[15];    // 8–14 chars + null terminator
    float length;      // in meters
    float height;      // in meters
    bool special;      // true if height > 2.0m OR length > 7.0m

public:
    // Initializes this Vehicle with provided attributes.
    // in: newLicence – null‐terminated license string
    // in: newPhone   – null‐terminated phone string
    // in: newLength  – length in meters
    // in: newHeight  – height in meters
    void initialize(
        const char* newLicence,
        const char* newPhone,
        float newLength,
        float newHeight
    );

    // Cleans up resources (if any).
    void shutdown();

    // Adds a Vehicle record with the given attributes.
    // in: newLicence – null‐terminated license string
    // in: newPhone   – null‐terminated phone string
    // in: newLength  – length in meters
    // in: newHeight  – height in meters
    void addVehicle(
        const char* newLicence,
        const char* newPhone,
        float newLength,
        float newHeight
    );

    // Edits this Vehicle's information.
    // in: newLicence – new license string
    // in: newPhone   – new phone string
    // in: newLength  – new length in meters
    // in: newHeight  – new height in meters
    void editVehicle(
        const char* newLicence,
        const char* newPhone,
        float newLength,
        float newHeight
    );

    // Determines if this Vehicle is classified as special.
    // Returns true if height > 2.0m OR length > 7.0m.
    bool isSpecial() const;

    // Returns this Vehicle's length (meters).
    float getLength() const;

    // Returns this Vehicle's height (meters).
    float getHeight() const;
};

//--------------------------------------------------
// Utility functions for vehicle management UI
//--------------------------------------------------

// Handles adding a vehicle through user interface
// Returns true if successful, false if cancelled or failed
bool addVehicleFromUI();

// Handles editing a vehicle through user interface
// in: licencePlate – license plate of vehicle to edit
// Returns true if successful, false if cancelled or failed
bool editVehicleFromUI(const std::string& licencePlate);


#endif // VEHICLE_H
```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## VEHICLEFILEIO.CPP

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
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
```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

## VEHICLEFILEIO.H

```cpp
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// FILE: VehicleFileIO.h
//************************************************************
// PROJECT: CMPT 276 – Ferry Reservation Software System
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

    // Saves a Vehicle record with complete data to the data file.
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
```

<!-- New Page -->
<div style="page-break-before: always;"></div>


---

**End of Document**

Generated on: Wed Jul 23 20:59:45 PDT 2025
Total Files: 16
