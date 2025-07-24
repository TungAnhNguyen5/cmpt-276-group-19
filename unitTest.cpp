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
}