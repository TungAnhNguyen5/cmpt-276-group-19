# Ship Happens – Ferry Reservation Software System (FRSS)

Welcome to **Ship Happens**, where innovation meets seamless ferry travel.

In partnership with **Coastal Ferry Corp (CFC)**, we proudly introduce **FRSS – Ferry Reservation Software System**, the modern solution for efficient and stress-free ferry bookings.

## Key Features

- Effortless booking schedule management  
- Real-time vehicle space availability  
- Contactless and streamlined ticketing  
- Enhanced terminal throughput and boarding speed  
- Role-based flows for both fleet managers and customers

## System Overview

FRSS transforms **CFC’s** legacy, first-come-first-served vehicle loading into a **reservation-first model**, delivering major benefits:

- Improved customer experience  
- Optimized vehicle deck allocation  
- Reduced terminal congestion  
- Faster and more efficient boarding

---

## Project Structure

```plaintext
.
├── header/
│   ├── reservation.h         # Reservation management class
│   ├── reservationFileIO.h   # I/O handling for reservation data
│   ├── sailing.h             # Sailing details and logic
│   ├── sailingFileIO.h       # I/O functions for sailings
│   ├── ui.h                  # User interface and prompts
│   ├── vehicle.h             # Vehicle class and classification
│   └── vehicleFileIO.h       # I/O operations for vehicle data
│
└── main/
    ├── main.cpp              # Entry point of the application
    └── submission.txt        # Project submission documentation
```

---

## Getting Started

### Prerequisites

- C++17 or later
- A terminal and C++ compiler (e.g., `g++` on Linux/macOS/WSL/MinGW)

### Compilation

From the root directory, compile the project using:

```bash
g++ -std=c++17 main/main.cpp -o ferry-reservation
```

### Run the Program

```bash
./ferry-reservation
```

---

## License

This project is **proprietary software** developed for academic use. Not intended for commercial distribution or deployment.

---

## Acknowledgements

Special thanks to **Coastal Ferry Corp (CFC)** for their collaboration, and to the **CMPT 276 development team** for their dedication to building a user-centric and forward-thinking ferry reservation system.
