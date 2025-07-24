#!/bin/bash

# generate_code_files.sh
# Generates a text file with all header and cpp files
# Format: main files first, then alphabetical order (cpp then header for same name)
# Each file starts with //@@@@@@@@@@@@@@@@@@@@@@@

OUTPUT_FILE="All_Source_Code.txt"

echo "Generating source code compilation..."

# Create the output file
echo "Ferry Reservation Software System - Source Code Compilation" > "$OUTPUT_FILE"
echo "Generated on: $(date)" >> "$OUTPUT_FILE"
echo "=============================================================" >> "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"

# Function to add a source file to the compilation
add_source_file() {
    local filepath="$1"
    local filename=$(basename "$filepath")
    
    if [[ -f "$filepath" ]]; then
        echo "" >> "$OUTPUT_FILE"
        echo "//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" >> "$OUTPUT_FILE"
        echo "// FILE: $filename" >> "$OUTPUT_FILE"
        echo "//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"
        cat "$filepath" >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"
    else
        echo "Warning: File $filepath not found"
    fi
}

# Add main.cpp first
echo "Adding main.cpp..."
add_source_file "main.cpp"

# Define base names for remaining files in alphabetical order
base_names=(
    "reservation"
    "reservationFileIO"
    "sailing"
    "sailingFileIO"
    "ui"
    "unitTest"
    "vehicle"
    "vehicleFileIO"
)

# Add files in alphabetical order (cpp then header for each base name)
echo "Adding remaining files in alphabetical order..."
for base in "${base_names[@]}"; do
    # Add .cpp file first if it exists
    if [[ -f "${base}.cpp" ]]; then
        echo "Adding ${base}.cpp..."
        add_source_file "${base}.cpp"
    fi
    
    # Then add .h file if it exists
    if [[ -f "${base}.h" ]]; then
        echo "Adding ${base}.h..."
        add_source_file "${base}.h"
    fi
done

echo "" >> "$OUTPUT_FILE"
echo "=============================================================" >> "$OUTPUT_FILE"
echo "End of Source Code Compilation" >> "$OUTPUT_FILE"
echo "Total files processed: $(grep -c "//@@@@@@@@@@@@@@@@@@@@@@@" "$OUTPUT_FILE")" >> "$OUTPUT_FILE"

echo "Source code compilation generated: $OUTPUT_FILE"
echo "Files included in order:"
echo "1. main.cpp"
count=2
for base in "${base_names[@]}"; do
    if [[ -f "${base}.cpp" ]]; then
        echo "$count. ${base}.cpp"
        ((count++))
    fi
    if [[ -f "${base}.h" ]]; then
        echo "$count. ${base}.h"
        ((count++))
    fi
done
