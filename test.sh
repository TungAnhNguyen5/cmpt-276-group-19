#!/bin/bash
# Ferry Reservation System - Complete Test Suite

echo "=== Ferry Reservation System - Complete Test Suite ==="
echo "Testing all major system features systematically"
echo ""

# Function to run a test and display results
run_test() {
    local test_name="$1"
    local input_sequence="$2"
    
    echo "----------------------------------------"
    echo "Testing: $test_name"
    echo "----------------------------------------"
    
    echo -e "$input_sequence" | timeout 10s ./ferry_system
    
    if [ $? -eq 124 ]; then
        echo "Test TIMEOUT: $test_name"
    else
        echo "Test COMPLETED: $test_name (exit code: $?)"
    fi
    echo ""
}

# Build the system first
echo "Building Ferry Reservation System..."
./build.sh
echo ""

# Generate unique timestamp-based ID to avoid conflicts
UNIQUE_ID=$(date +%H%M%S | cut -c1-3)
UNIQUE_DAY=$(date +%d)
UNIQUE_TIME=$(date +%H)

# Test 4.01: Add Sailing - Use truly unique ID to avoid conflicts
echo "TEST 4.01: Add Sailing (ID: $UNIQUE_ID-$UNIQUE_DAY-$UNIQUE_TIME)"
run_test "Add New Sailing" "1\n1\n$UNIQUE_ID\n$UNIQUE_DAY\n$UNIQUE_TIME\nVessel$UNIQUE_ID\n3\n8\n1\n0\n0"

# Test 4.04: Add Vehicle - Use unique license plate
echo "TEST 4.04: Add Vehicle (License: V$UNIQUE_ID)"
run_test "Add New Vehicle" "2\n1\nV$UNIQUE_ID\n98765$UNIQUE_ID\n2.5\n10\n1\n0\n0"

# Test 4.05: Add Reservation - Use the sailing and vehicle we just created
echo "TEST 4.05: Add Reservation (Sailing: $UNIQUE_ID-$UNIQUE_DAY-$UNIQUE_TIME)"  
run_test "Add New Reservation" "3\n$UNIQUE_ID-$UNIQUE_DAY-$UNIQUE_TIME\n1\nV$UNIQUE_ID\n0\n0"

# Test 4.08: Display Sailing Report
echo "TEST 4.08: Sailing Report"
run_test "Display Sailing Report" "4\n0\n0"

echo "=========================================="
echo "COMPLETE TEST SUITE FINISHED"
echo "=========================================="