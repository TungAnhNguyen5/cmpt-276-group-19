# Ship Happens – Ferry Reservation Software System (FRSS)

Welcome to **Ship Happens**, where innovation meets seamless ferry travel.

We are proud to partner with **Coastal Ferry Corp (CFC)** to introduce **FRSS – Ferry Reservation Software System**, the ultimate solution for simplifying ferry bookings.

With FRSS, you can:
- Manage booking schedules effortlessly  
- View real-time vehicle space availability  
- Secure ticketing without tedious lineups  

Say goodbye to manual systems and hello to a smoother, more efficient way of handling ferry reservations. Whether you’re a **fleet manager** or a **customer**, FRSS ensures every journey is hassle-free.

---

## System Overview

FRSS is developed to transition **CFC** from a first-come, first-served vehicle loading system to a **modern reservation-based model**. This upgrade improves:

- Customer experience  
- Vehicle space management  
- Terminal congestion reduction  
- Boarding efficiency  

---

## Project Structure

```
.
├── main.cpp                   # Entry point of the application
├── ui.cpp/h                   # User interface implementation
├── sailing.cpp/h              # Sailing management and logic
├── sailingFileIO.cpp/h        # I/O functions for sailings
├── vehicle.cpp/h              # Vehicle class and classification
├── vehicleFileIO.cpp/h        # I/O operations for vehicle data
├── reservation.cpp/h          # Reservation management class
├── reservationFileIO.cpp/h    # I/O handling for reservation data
├── unitTest.cpp               # Unit tests for reservation file I/O
├── setup_test_data.cpp        # Demo data generation utility
├── check_demo_data.cpp        # Data verification utility
├── build.sh                   # Automated build script
├── generate_code_files.sh     # Source code compilation generator
├── Makefile                   # Build configuration
├── ferry_system               # Main executable (compiled)
├── unit_test                  # Unit test executable (compiled)
├── setup_demo                 # Demo data setup executable
├── All_Source_Code.txt        # Complete source code compilation
└── README.md                  # Project documentation
```

---

## Getting Started

### Prerequisites

- C++11 or higher
- A terminal and `g++` compiler (e.g., on Linux/Mac/WSL/MinGW)
- Make utility (optional, for Makefile usage)

### Quick Start

The easiest way to build and run the system:

```bash
# Option 1: Using the automated build script
./build.sh

# Option 2: Using Make (recommended)
make clean && make

# Set up demo data (recommended for first-time users)
./setup_demo

# Run the main ferry reservation system
./ferry_system

# Run unit tests
./unit_test

# Generate complete source code compilation
./generate_code_files.sh
```

### Manual Compilation

If you prefer to compile manually:

```bash
# Using g++ directly (main system)
g++ -std=c++11 -Wall -Wextra -g main.cpp ui.cpp sailing.cpp sailingFileIO.cpp vehicle.cpp vehicleFileIO.cpp reservation.cpp reservationFileIO.cpp -o ferry_system

# Using g++ directly (unit test)
g++ -std=c++11 -Wall -Wextra -g unitTest.cpp reservationFileIO.cpp -o unit_test

# Using g++ directly (demo setup)
g++ -std=c++11 -Wall -Wextra -g setup_test_data.cpp ui.cpp sailing.cpp sailingFileIO.cpp vehicle.cpp vehicleFileIO.cpp reservation.cpp reservationFileIO.cpp -o setup_demo
```

### System Features

**Main Ferry System (`./ferry_system`):**
- **Manage Sailings**: Add, edit, and view sailing schedules with comprehensive validation
- **Manage Vehicles**: Add, edit vehicles with automatic special vehicle classification
- **Manage Reservations**: Create, check-in, and manage reservations with capacity tracking
- **Sailing Reports**: Generate detailed sailing reports with utilization percentages
- **Advanced Input Validation**: Format guidance and constraints for all data entry
- **Realistic Capacity Calculation**: Uses actual vehicle dimensions and 0.5m spacing
- **Binary File I/O**: Efficient data persistence with robust error handling

**Demo Data Setup (`./setup_demo`):**
- Generates sample sailing schedules and vehicle data
- Creates realistic test scenarios for system validation
- Prepares the system for immediate use

**Unit Testing (`./unit_test`):**
- Comprehensive tests for reservation file I/O operations
- Validates data integrity and persistence functions
- Ensures system reliability and correctness

**Code Generation (`./generate_code_files.sh`):**
- Creates complete source code compilation in `All_Source_Code.txt`
- Organized file structure with clear separators
- Includes all header and implementation files

### Usage Notes

- The system operates through numbered menus
- Type a number and press Enter to select options
- Type 0 or 'Cancel' to go back at any time
- All data is automatically persisted to binary files
- Comprehensive format guidance is provided for all data entry
- Vehicle classification: Special vehicles (height > 2.0m OR length > 7.0m)
- Regular vehicles default to 7.0m × 2.0m dimensions
- Capacity calculations include 0.5m spacing between vehicles
- All loops include goal comments for code clarity

### Code Quality Features

**Documentation Standards:**
- Every loop includes a clear goal comment explaining its purpose
- Comprehensive format guidance for all user input fields
- Detailed function headers with purpose and parameter descriptions
- Consistent coding style throughout the project

**Input Validation:**
- License plates: Alphanumeric, 1-10 characters
- Phone numbers: Exactly 10 digits
- Vehicle dimensions: Float with one decimal place
- Sailing IDs: Terminal code + date + time format
- Vessel IDs: 1-25 characters, no spaces

**Data Integrity:**
- Binary file format for efficient storage
- Automatic backup and recovery mechanisms
- Robust error handling for file operations
- Validation at multiple system layers

---

## Release Information

**Current Release**: Final Submission - August 5, 2025

### What's New in This Release
- **Enhanced Code Documentation**: All loops now include goal comments for clarity
- **Comprehensive Input Validation**: Detailed format guidance for all user inputs
- **Realistic Vehicle Capacity**: Accurate calculations using actual dimensions and spacing
- **Improved Error Handling**: Robust validation and user-friendly error messages
- **Code Generation Tool**: Automated source code compilation generator
- **Advanced Build System**: Makefile support with multiple build targets
- **Demo Data Integration**: Complete setup utilities for testing and demonstration

### Previous Features
- **Complete UI Integration**: Seamless integration with sailing and vehicle management modules
- **Real Data Display**: Dynamic vehicle counts and sailing information from file I/O
- **Binary File I/O**: Efficient data persistence and retrieval
- **Unit Testing**: Comprehensive testing for reservation file operations
- **Automated Build**: Multiple build options for easy compilation

### Executables Included
- `ferry_system` - Main application (~764KB)
- `unit_test` - Unit test suite
- `setup_demo` - Demo data generation utility
- `build.sh` - Automated build script
- `generate_code_files.sh` - Source code compilation generator

---

 License
This project is proprietary software developed for academic purposes and is not intended for commercial distribution.

 Acknowledgements

Special thanks to Coastal Ferry Corp (CFC) for their collaboration, and to the **CMPT 276 Group 19** development team for designing a user-friendly and future-ready ferry reservation system.

**Development Team**: Group 19  
**Project Date**: August 5, 2025  
**Course**: CMPT 276 - Software Engineering  
**Final Submission**: Complete system with enhanced documentation and robust features
