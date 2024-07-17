# Compiler
CXX = g++

# Directories
INCLUDE_DIR = src/include
BUILD_DIR = build
BIN_DIR = bin

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -I$(INCLUDE_DIR) $(shell wx-config --cxxflags) $(shell pkg-config --cflags --libs webkit2gtk-4.1)
LDFLAGS = $(shell wx-config --libs webview) $(shell wx-config --libs) $(shell pkg-config --cflags --libs webkit2gtk-4.1)

# Source files
SRC_DIR = src/cpp
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

# Executable name
TARGET = output
OUTPUT_FILE = $(BIN_DIR)/$(TARGET)

# Default target
all: $(OUTPUT_FILE)

# Link the executable
$(OUTPUT_FILE): $(OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(BUILD_DIR)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Create bin directory for executable
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean up build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean

# Run Target (assuming you resolve permission issues)
run: $(OUTPUT_FILE)
	./$(OUTPUT_FILE)

debug: $(OUTPUT_FILE)
	GTK_DEBUG=interactive ./$(OUTPUT_FILE)
