# 🚢 Ship Happens – Ferry Reservation Software System (FRSS)

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
├── main.cpp                # Entry point of the application
├── ui.cpp/h                # User interface implementation
├── sailing.cpp/h           # Sailing management and logic
├── sailingFileIO.cpp/h     # I/O functions for sailings
├── vehicle.cpp/h           # Vehicle class and classification
├── vehicleFileIO.cpp/h     # I/O operations for vehicle data
├── reservation.cpp/h       # Reservation management class
├── reservationFileIO.cpp/h # I/O handling for reservation data
├── unitTest.cpp            # Unit tests for reservation file I/O
├── build.sh                # Automated build script
├── ferry_system            # Main executable (compiled)
├── unit_test               # Unit test executable (compiled)
└── README.md               # Project documentation
```

---

## Getting Started

### Prerequisites

- C++17 or higher
- A terminal and `g++` compiler (e.g., on Linux/Mac/WSL/MinGW)

### Quick Start

The easiest way to build and run the system:

```bash
# Build everything (main system + unit tests)
./build.sh

# Run the main ferry reservation system
./ferry_system

# Run unit tests
./unit_test
```

### Manual Compilation

If you prefer to compile manually:

```bash
# Compile main system
g++ -fdiagnostics-color=always -g main.cpp ui.cpp sailing.cpp sailingFileIO.cpp vehicle.cpp vehicleFileIO.cpp reservation.cpp reservationFileIO.cpp -o ferry_system

# Compile unit test
g++ -fdiagnostics-color=always -g unitTest.cpp reservationFileIO.cpp -o unit_test
```

### System Features

**Main Ferry System (`./ferry_system`):**
- **Manage Sailings**: Add, edit, and view sailing schedules with real data integration
- **Manage Vehicles**: Add, edit vehicles with automatic special vehicle classification
- **Manage Reservations**: Create, edit, and check-in reservations
- **Sailing Reports**: Generate comprehensive sailing reports
- **User-Friendly Interface**: Step-by-step guidance with format hints

**Unit Testing (`./unit_test`):**
- Tests reservation file I/O operations
- Validates saveReservation() and getReservation() functions
- Ensures data integrity for the reservation system

### Usage Notes

- The system operates through numbered menus
- Type a number and press Enter to select options
- Type 0 or 'Cancel' to go back at any time
- All data is automatically persisted to files
- Format guidance is provided for all data entry
- Special vehicles are automatically classified (height > 2.0m OR length > 7.0m)
---

## Release Information

**Current Release**: Latest Build - July 23, 2025

### What's New
- ✅ **Complete UI Integration**: Seamless integration with sailing and vehicle management modules
- ✅ **Real Data Display**: Dynamic vehicle counts and sailing information from file I/O
- ✅ **Format Guidance**: User-friendly input hints for all data entry fields
- ✅ **Automated Build**: One-command build script for easy compilation
- ✅ **Unit Testing**: Comprehensive testing for reservation file operations
- ✅ **Error Handling**: Graceful fallbacks when data files are unavailable

### Executables Included
- `ferry_system` - Main application (602KB)
- `unit_test` - Unit test suite (168KB)
- `build.sh` - Automated build script

---

📜 License
This project is proprietary software developed for academic purposes and is not intended for commercial distribution.

🤝 Acknowledgements

Special thanks to Coastal Ferry Corp (CFC) for their collaboration, and to the **CMPT 276 Group 19** development team for designing a user-friendly and future-ready ferry reservation system.

**Development Team**: Group 19  
**Project Date**: July 23, 2025  
**Course**: CMPT 276 - Software Engineering
