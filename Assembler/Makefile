# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wno-all -Wextra -std=c99

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
INPUT_DIR := input
OUTPUT_DIR := output

# Source files and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Header files
DEPS := $(wildcard $(SRC_DIR)/*.h)

# Executable name
EXEC := riscv_asm
EXEC_PATH := $(BIN_DIR)/$(EXEC)

# Input and output file names
INPUT_FILE := input.s
OUTPUT_FILE := output.hex

# Commands
MKDIR := mkdir -p
RM := rm -rf
CP := cp
MV := mv

# Phony targets
.PHONY: all clean run dirs 

# Default target
all: dirs $(EXEC_PATH)

# Create necessary directories
dirs:
	@$(MKDIR) $(BIN_DIR) $(OBJ_DIR) $(OUTPUT_DIR)

# Rule to create object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the executable
$(EXEC_PATH): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

# Clean rule
clean:
	$(RM) $(OBJ_DIR) $(BIN_DIR) $(INPUT_FILE) $(EXEC)

# Run rule
run: all
	$(CP) $(EXEC_PATH) .
	$(CP) $(INPUT_DIR)/$(INPUT_FILE) .
	./$(EXEC)
	$(MV) $(OUTPUT_FILE) $(OUTPUT_DIR)/

# Include dependencies
-include $(OBJ_FILES:.o=.d)