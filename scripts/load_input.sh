#!/bin/bash

# This script processes the input.s file and prepares input.hex for the simulator

# Check if the correct command is used

# if [ "\$1 \$2" != "load input.s" ]; then
#     echo "Usage: \$0 load input.s"
#     exit 1
# fi

# Set up directory variables
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo "Script directory: $SCRIPT_DIR"
PROJECT_ROOT="$SCRIPT_DIR/.."
echo "Project root: $PROJECT_ROOT"
INPUT_DIR="$PROJECT_ROOT/input"
echo "Input directory: $INPUT_DIR"
ASSEMBLER_DIR="$PROJECT_ROOT/Assembler"
echo "Assembler directory: $ASSEMBLER_DIR"

# Copy input.s to Assembler input folder
cp "$INPUT_DIR/input.s" "$ASSEMBLER_DIR/input/input.s"

# Change to Assembler directory and run make commands
cd "$ASSEMBLER_DIR"
make clean
make
make run

# Copy and rename output.hex
cp "$ASSEMBLER_DIR/output/output.hex" "$INPUT_DIR/input.hex"

# Print success message
echo "Processing complete. input.hex has been generated in the input folder."