#!/bin/bash

# generate_code_files.sh
# Generates a text file with all header and cpp files
# Format: main.cpp first, then pairs of .h+.cpp files in alphabetical order (.h before .cpp)
# Unit tests at the end. Each file starts with //@@@@@@@@@@@@@@@@@@@@@@@

OUTPUT_FILE="All_Source_Code.txt"

echo "Generating source code compilation..."

# Create the output file
echo "Ferry Reservation Software System - Source Code Compilation" > "$OUTPUT_FILE"
echo "Generated on: $(date)" >> "$OUTPUT_FILE"
echo "=============================================================" >> "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"

# Function to add a source file to the compilation (with page break)
add_source_file() {
    local filepath="$1"
    local filename=$(basename "$filepath")
    
    if [[ -f "$filepath" ]]; then
        echo -e "\f" >> "$OUTPUT_FILE"  # Form feed for new page
        echo "//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" >> "$OUTPUT_FILE"
        echo "// FILE: $filename" >> "$OUTPUT_FILE"
        echo "//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" >> "$OUTPUT_FILE"
        cat "$filepath" >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"
    else
        echo "Warning: File $filepath not found"
    fi
}

# Add main.cpp first
echo "Adding main.cpp..."
add_source_file "main.cpp"

# Define base names for pairs in alphabetical order (excluding main and unit tests)
base_names=(
    "reservation"
    "reservationFileIO"
    "sailing"
    "sailingFileIO"
    "ui"
    "vehicle"
    "vehicleFileIO"
)

# Add files in alphabetical order (.h then .cpp for each pair)
echo "Adding header/implementation pairs in alphabetical order..."
for base in "${base_names[@]}"; do
    # Add .h file first if it exists
    if [[ -f "${base}.h" ]]; then
        echo "Adding ${base}.h..."
        add_source_file "${base}.h"
    fi
    
    # Then add .cpp file if it exists
    if [[ -f "${base}.cpp" ]]; then
        echo "Adding ${base}.cpp..."
        add_source_file "${base}.cpp"
    fi
done

# Add unit tests at the end
echo "Adding unit tests..."
if [[ -f "unitTest.cpp" ]]; then
    echo "Adding unitTest.cpp..."
    add_source_file "unitTest.cpp"
fi

# Add setup/demo files if they exist
if [[ -f "setup_test_data.cpp" ]]; then
    echo "Adding setup_test_data.cpp..."
    add_source_file "setup_test_data.cpp"
fi

if [[ -f "check_demo_data.cpp" ]]; then
    echo "Adding check_demo_data.cpp..."
    add_source_file "check_demo_data.cpp"
fi

echo "" >> "$OUTPUT_FILE"
echo "=============================================================" >> "$OUTPUT_FILE"
echo "End of Source Code Compilation" >> "$OUTPUT_FILE"
echo "Total files processed: $(grep -c "//@@@@@@@@@@@@@@@@@@@@@@@" "$OUTPUT_FILE")" >> "$OUTPUT_FILE"

echo "Source code compilation generated: $OUTPUT_FILE"
echo "Files included in order:"
echo "1. main.cpp"
count=2
for base in "${base_names[@]}"; do
    if [[ -f "${base}.h" ]]; then
        echo "$count. ${base}.h"
        ((count++))
    fi
    if [[ -f "${base}.cpp" ]]; then
        echo "$count. ${base}.cpp"
        ((count++))
    fi
done

# Show unit test files
if [[ -f "unitTest.cpp" ]]; then
    echo "$count. unitTest.cpp"
    ((count++))
fi
if [[ -f "setup_test_data.cpp" ]]; then
    echo "$count. setup_test_data.cpp"
    ((count++))
fi
if [[ -f "check_demo_data.cpp" ]]; then
    echo "$count. check_demo_data.cpp"
    ((count++))
fi
