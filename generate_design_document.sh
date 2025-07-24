#!/bin/bash

# generate_design_document.sh
# Generates a detailed design document for the Ferry Reservation Software System
# Including all source code files in courier font with proper formatting

OUTPUT_FILE="Ferry_Reservation_System_Design_Document.md"

echo "Generating Ferry Reservation System Design Document..."

# Create the design document
cat > "$OUTPUT_FILE" << 'EOF'
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

EOF

# Function to add a source file to the document
add_source_file() {
    local filepath="$1"
    local filename=$(basename "$filepath")
    
    if [[ -f "$filepath" ]]; then
        echo "" >> "$OUTPUT_FILE"
        echo "---" >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"
        echo "## $(echo "$filename" | tr '[:lower:]' '[:upper:]')" >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"
        echo '```cpp' >> "$OUTPUT_FILE"
        echo "//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" >> "$OUTPUT_FILE"
        cat "$filepath" >> "$OUTPUT_FILE"
        echo '```' >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"
        echo '<!-- New Page -->' >> "$OUTPUT_FILE"
        echo '<div style="page-break-before: always;"></div>' >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"
    else
        echo "Warning: File $filepath not found"
    fi
}

# Add main files first
echo "Adding main files..."
add_source_file "main.cpp"

# Add remaining files in alphabetical order
echo "Adding remaining files in alphabetical order..."

# Get all relevant source files and sort them alphabetically
files=(
    "reservation.cpp"
    "reservation.h"
    "reservationFileIO.cpp"
    "reservationFileIO.h"
    "sailing.cpp"
    "sailing.h"
    "sailingFileIO.cpp"
    "sailingFileIO.h"
    "ui.cpp"
    "ui.h"
    "unitTest.cpp"
    "vehicle.cpp"
    "vehicle.h"
    "vehicleFileIO.cpp"
    "vehicleFileIO.h"
)

for file in "${files[@]}"; do
    if [[ -f "$file" ]]; then
        add_source_file "$file"
    fi
done

echo "" >> "$OUTPUT_FILE"
echo "---" >> "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"
echo "**End of Document**" >> "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"
echo "Generated on: $(date)" >> "$OUTPUT_FILE"
echo "Total Files: $((${#files[@]} + 1))" >> "$OUTPUT_FILE"

echo "Design document generated: $OUTPUT_FILE"
echo "Total files included: $((${#files[@]} + 1))"
