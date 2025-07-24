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

.
├── header/
│ ├── reservation.h         # Reservation management class
│ ├── reservationFileIO.h   # I/O handling for reservation data
│ ├── sailing.h             # Sailing details and logic
│ ├── sailingFileIO.h       # I/O functions for sailings
│ ├── ui.h                  # User interface and prompts
│ ├── vehicle.h             # Vehicle class and classification
│ └── vehicleFileIO.h       # I/O operations for vehicle data
│
└── main/
├── main.cpp                # Entry point of the application
└── submission.txt          # Project submission documentation

yaml
Copy
Edit

---

## Getting Started

### Prerequisites

- C++17 or higher
- A terminal and `g++` compiler (e.g., on Linux/Mac/WSL/MinGW)

### Compilation

Run the following command from the project root:

```bash
g++ -std=c++17 main/main.cpp -o ferry-reservation
Run the Program
bash
Copy
Edit
./ferry-reservation
📜 License
This project is proprietary software developed for academic purposes and is not intended for commercial distribution.

🤝 Acknowledgements
Special thanks to Coastal Ferry Corp (CFC) for their collaboration, and to the CMPT 276 development team for designing a user-friendly and future-ready ferry reservation system.

yaml
Copy
Edit

---

Would you like me to auto-generate a downloadable file for this?
