CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -pedantic -I./include
LDFLAGS =

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INPUT_DIR = input

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/simulator
INPUT_FILE = $(INPUT_DIR)/input.hex

.PHONY: all clean run

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: $(EXECUTABLE)
		./$(EXECUTABLE)
	

-include $(OBJECTS:.o=.d)

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -MM -MT $(@:.d=.o) $< > $@