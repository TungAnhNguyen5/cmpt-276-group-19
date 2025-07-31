#!/bin/bash
# Ferry Reservation Sy# Test 4.01: Add Sailing

echo "=== Ferry Reservation System - Full Feature Test ==="
echo "Testing all major system features systematically"
echo ""

# Function to run a test and display results
run_test() {
    local test_name="$1"
    local input_sequence="$2"
    
    echo "----------------------------------------"
    echo "Testing: $test_name"
    echo "----------------------------------------"
    
    echo -e "$input_sequence" | timeout 5s ./ferry_system
    
    if [ $? -eq 124 ]; then
        echo "Test TIMEOUT: $test_name"
    else
        echo "Test EXITED: $test_name (exit code: $?)"
    fi
    echo ""
}

# Build the system first
echo "Building Ferry Reservation System..."
./build.sh
echo ""

# Test 4.01: Add Sailing
echo "TEST 4.01: Add Sailing"
run_test "Add New Sailing" "1\n1\nAGA\n15\n09\nAirship263\n8\n16\n1\n0\n0"

# Test 4.04: Add Vehicle  
echo "TEST 4.04: Add Vehicle"
run_test "Add New Vehicle" "2\n1\nTEST999\n1231231234\n2.5\n10\n1\n0\n0"

# Test 4.05: Add Reservation
echo "TEST 4.05: Add Reservation"  
run_test "Add New Reservation" "3\n1\nAGA-15-09\nTEST999\n1231231234\n1\n0\n0"

# Test 4.08: Display Sailing Report
echo "TEST 4.08: Sailing Report"
run_test "Display Sailing Report" "4\n0\n0"

echo "=========================================="
echo "CORE FUNCTIONALITY TEST COMPLETE"
echo "=========================================="