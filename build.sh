#!/bin/bash

# Build script for Ferry Reservation System
# CMPT 276 - Group 19

echo "Building Ferry Reservation System..."

# Compile main ferry system
echo "Compiling main system..."
g++ -fdiagnostics-color=always -g main.cpp ui.cpp sailing.cpp sailingFileIO.cpp vehicle.cpp vehicleFileIO.cpp reservation.cpp reservationFileIO.cpp -o ferry_system

if [ $? -eq 0 ]; then
    echo "✓ Main system compiled successfully -> ferry_system"
else
    echo "✗ Main system compilation failed"
    exit 1
fi

# Compile unit test
echo "Compiling unit test..."
g++ -fdiagnostics-color=always -g unitTest.cpp reservationFileIO.cpp -o unit_test

if [ $? -eq 0 ]; then
    echo "✓ Unit test compiled successfully -> unit_test"
else
    echo "✗ Unit test compilation failed"
    exit 1
fi

echo ""
echo "Build complete! Executables created:"
echo "  - ferry_system (main application)"
echo "  - unit_test (reservation file I/O unit test)"
echo ""
echo "To run:"
echo "  ./ferry_system    # Start the ferry reservation system"
echo "  ./unit_test       # Run the unit tests"
