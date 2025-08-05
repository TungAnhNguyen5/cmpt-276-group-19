# Makefile for Ferry Reservation System
# CMPT 276 - Group 19

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Target executables
MAIN_TARGET = ferry_system
UNIT_TEST_TARGET = unit_test
SETUP_TARGET = setup_demo

# Source files
MAIN_SRC = main.cpp
UNIT_TEST_SRC = unitTest.cpp
SETUP_SRC = setup_test_data.cpp

# Object files (exclude main files to avoid multiple main() definitions)
OBJECTS = ui.o sailing.o sailingFileIO.o vehicle.o vehicleFileIO.o reservation.o reservationFileIO.o

# Header files (for dependency tracking)
HEADERS = ui.h sailing.h sailingFileIO.h vehicle.h vehicleFileIO.h reservation.h reservationFileIO.h

# Default target
all: $(MAIN_TARGET) $(UNIT_TEST_TARGET) $(SETUP_TARGET)

# Main ferry system executable
$(MAIN_TARGET): $(MAIN_SRC) $(OBJECTS)
	@echo "Building Ferry Reservation System..."
	$(CXX) $(CXXFLAGS) -o $(MAIN_TARGET) $(MAIN_SRC) $(OBJECTS)
	@echo "✓ Main system compiled successfully -> $(MAIN_TARGET)"

# Unit test executable
$(UNIT_TEST_TARGET): $(UNIT_TEST_SRC) reservationFileIO.o
	@echo "Compiling unit test..."
	$(CXX) $(CXXFLAGS) -o $(UNIT_TEST_TARGET) $(UNIT_TEST_SRC) reservationFileIO.o
	@echo "✓ Unit test compiled successfully -> $(UNIT_TEST_TARGET)"

# Setup demo data executable
$(SETUP_TARGET): $(SETUP_SRC) $(OBJECTS)
	@echo "Compiling demo data setup..."
	$(CXX) $(CXXFLAGS) -o $(SETUP_TARGET) $(SETUP_SRC) $(OBJECTS)
	@echo "✓ Demo setup compiled successfully -> $(SETUP_TARGET)"

# Object file compilation rules
ui.o: ui.cpp ui.h sailing.h vehicle.h vehicleFileIO.h reservation.h reservationFileIO.h
	$(CXX) $(CXXFLAGS) -c ui.cpp

sailing.o: sailing.cpp sailing.h sailingFileIO.h ui.h
	$(CXX) $(CXXFLAGS) -c sailing.cpp

sailingFileIO.o: sailingFileIO.cpp sailingFileIO.h sailing.h
	$(CXX) $(CXXFLAGS) -c sailingFileIO.cpp

vehicle.o: vehicle.cpp vehicle.h vehicleFileIO.h
	$(CXX) $(CXXFLAGS) -c vehicle.cpp

vehicleFileIO.o: vehicleFileIO.cpp vehicleFileIO.h vehicle.h
	$(CXX) $(CXXFLAGS) -c vehicleFileIO.cpp

reservation.o: reservation.cpp reservation.h reservationFileIO.h
	$(CXX) $(CXXFLAGS) -c reservation.cpp

reservationFileIO.o: reservationFileIO.cpp reservationFileIO.h reservation.h
	$(CXX) $(CXXFLAGS) -c reservationFileIO.cpp

# Convenience targets
build: all
	@echo ""
	@echo "Build complete! Executables created:"
	@echo "  • $(MAIN_TARGET) - Main ferry reservation system"
	@echo "  • $(UNIT_TEST_TARGET) - Unit tests for reservation file I/O"
	@echo "  • $(SETUP_TARGET) - Demo data setup script"
	@echo ""
	@echo "To run:"
	@echo "  ./$(SETUP_TARGET)     # Set up demo data first"
	@echo "  ./$(MAIN_TARGET)      # Run the main system"
	@echo "  ./$(UNIT_TEST_TARGET) # Run unit tests"

# Set up demo data (clears existing data and creates test data)
setup: $(SETUP_TARGET)
	@echo "Setting up demo data for testing..."
	./$(SETUP_TARGET)

# Run the main system
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Run unit tests
test: $(UNIT_TEST_TARGET)
	@echo "Running unit tests..."
	./$(UNIT_TEST_TARGET)

# Full demo preparation (setup + run)
demo: setup run

# Clean all generated files
clean:
	@echo "Cleaning up..."
	rm -f *.o
	rm -f $(MAIN_TARGET) $(UNIT_TEST_TARGET) $(SETUP_TARGET)
	@echo "Object files and executables removed"

# Clean data files only (keep executables)
clean-data:
	@echo "Cleaning data files..."
	rm -f sailingData.dat vehicles.dat reservation.dat *.dat
	@echo "✓ Data files and .dat files removed"

# Clean everything (executables and data)
clean-all: clean clean-data
	@echo "All files cleaned"

# Rebuild everything from scratch
rebuild: clean-all all

# Debug build with extra debugging symbols
debug: CXXFLAGS += -DDEBUG -O0
debug: clean all

# Release build with optimizations
release: CXXFLAGS += -O2 -DNDEBUG
release: clean all

# Show help
help:
	@echo "Ferry Reservation System - Makefile Help"
	@echo "========================================"
	@echo ""
	@echo "Building:"
	@echo "  make / make all      - Build all executables"
	@echo "  make build          - Build with status messages"
	@echo "  make debug          - Build with debug symbols"
	@echo "  make release        - Build optimized version"
	@echo "  make rebuild        - Clean and rebuild everything"
	@echo ""
	@echo "Running:"
	@echo "  make setup          - Set up demo data"
	@echo "  make run            - Run the main system"
	@echo "  make test           - Run unit tests"
	@echo "  make demo           - Setup data + run system"
	@echo ""
	@echo "Cleaning:"
	@echo "  make clean          - Remove object files and executables"
	@echo "  make clean-data     - Remove data files (.txt, .dat) only"
	@echo "  make clean-all      - Remove everything (executables + data)"
	@echo ""
	@echo "Individual targets:"
	@echo "  $(MAIN_TARGET)           - Main ferry system"
	@echo "  $(UNIT_TEST_TARGET)        - Unit test executable"
	@echo "  $(SETUP_TARGET)        - Demo data setup"

# Declare phony targets
.PHONY: all build setup run test demo clean clean-data clean-all rebuild debug release help

# Prevent deletion of object files
.PRECIOUS: $(OBJECTS)