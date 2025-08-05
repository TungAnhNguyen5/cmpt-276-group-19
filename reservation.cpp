// PROJECT: CMPT 276 – Ferry Reservation Software System //(Assignment #4)
// TEAM: Group 19
// DATE: 25/07/24
//************************************************************
// PURPOSE:
//   Implements reservation logic including adding, checking-in,
//   fare calculation, and reservation management.
//************************************************************
// USAGE:
// - Call initialize() before using any functions in this //module.
// - Call shutdown() before program exit to flush and close //storage.
//************************************************************
// REVISION HISTORY:
// Rev. 1 - 2025/07/24 - Danny Choi
//        - Initial .cpp implementation based on header 
// specification.
// Rev. 2 - 2025/08/05 - Danny Choi
//        - Fully debugged final release version.
//************************************************************


#include "reservation.h"
#include "reservationFileIO.h"
#include "sailing.h"
#include "sailingFileIO.h"
#include "vehicle.h"
#include "vehicleFileIO.h"
#include <iostream>
#include <cstring>

//--------------------------------------------------
// Function: initialize
//--------------------------------------------------
// Function: initialize
//--------------------------------------------------
// Opens the reservation file to initialize the system.
// Required before any other reservation operations can occur.
void initialize()
{
    open("reservation.dat");
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
    // Mark unused parameters to suppress compiler warnings
 
    
    // Get the sailing to update its capacity
    Sailing sailing = Sailing::getSailingFromIO(sailingID.c_str());
    
    // Check how many vehicles are already on this sailing to determine spacing
    std::vector<ReservationRecord> existingReservations = getAllOnSailing(sailingID);
    const float VEHICLE_SPACING = 0.5f; // 0.5 meter spacing between vehicles
    
    // Only add spacing if there are already vehicles on the sailing
    float spacingNeeded = existingReservations.empty() ? 0.0f : VEHICLE_SPACING;
    
    bool capacityUpdated = false;
    if (isSpecial) {
        // Special vehicles use:
        // - Length + spacing from LRL (Low Remaining Length)
        // - Height from HRL (High Remaining Length)
        float lengthNeeded = length + spacingNeeded;
        float heightNeeded = height;
        
        bool lrlUpdated = sailing.lrlUpdate(lengthNeeded);
        bool hrlUpdated = sailing.hrlUpdate(heightNeeded);
        capacityUpdated = lrlUpdated && hrlUpdated;
    } else {
        // Regular vehicles always use standard dimensions:
        // - 7.0m length + spacing from LRL (Low Remaining Length)  
        // - 2.0m height from HRL (High Remaining Length)
        const float REGULAR_LENGTH = 7.0f;
        const float REGULAR_HEIGHT = 2.0f;
        
        float lengthNeeded = REGULAR_LENGTH + spacingNeeded;
        float heightNeeded = REGULAR_HEIGHT;
        
        bool lrlUpdated = sailing.lrlUpdate(lengthNeeded);
        bool hrlUpdated = sailing.hrlUpdate(heightNeeded);
        capacityUpdated = lrlUpdated && hrlUpdated;
    }
    
    // If we couldn't update capacity (sailing is full), return false
    if (!capacityUpdated) {
        return false;
    }

    // Save the updated sailing capacity to file
    sailingFileIO::saveSailing(sailing);

    ReservationRecord record;
    std::memset(&record, 0, sizeof(ReservationRecord)); // Clear all bytes in the struct

    std::strncpy(record.sailingID, sailingID.c_str(), SAILING_ID_MAX - 1);
    record.sailingID[SAILING_ID_MAX - 1] = '\0'; // Ensure null termination
    
    std::strncpy(record.licensePlate, licensePlate.c_str(), LICENSE_PLATE_MAX - 1);
    record.licensePlate[LICENSE_PLATE_MAX - 1] = '\0'; // Ensure null termination

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
    const float REGULAR_FARE = 14.00f;
    const float HEIGHT_THRESHOLD = 2.00f;
    const float OVER_HEIGHT_RATE = 3.00f;
    const float STANDARD_RATE = 2.00f;

    if (!isSpecial)
        return REGULAR_FARE;

    if (height > HEIGHT_THRESHOLD)
        return length * OVER_HEIGHT_RATE;

    else if (height <= HEIGHT_THRESHOLD)
        return length * STANDARD_RATE;

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

    // Loop goal: Delete each reservation record for the specified sailing
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

    // Loop goal: Move each reservation from one sailing to another by updating sailingID
    for (auto &rec : records)
    {
        std::strncpy(rec.sailingID, toSailingID.c_str(), SAILING_ID_MAX);

        if (saveReservation(rec))
        {
            ++count;
        }
    }

    return count;
}