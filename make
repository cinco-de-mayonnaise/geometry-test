# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c11
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = 

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files (add your .c and .cpp files here)
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES)))

# Target executable
TARGET = $(BIN_DIR)/main

# SDL library flags
SDL_CFLAGS = $(shell pkg-config --cflags sdl2)
SDL_LDFLAGS = $(shell pkg-config --libs sdl2)

# Add other libraries here (e.g., SDL_image, SDL_ttf, etc.)
# EXTRA_LIBS_CFLAGS = $(shell pkg-config --cflags sdl2_image)
# EXTRA_LIBS_LDFLAGS = $(shell pkg-config --libs sdl2_image)

# Combine all flags
CFLAGS += $(SDL_CFLAGS) $(EXTRA_LIBS_CFLAGS)
CXXFLAGS += $(SDL_CFLAGS) $(EXTRA_LIBS_CFLAGS)
LDFLAGS += $(SDL_LDFLAGS) $(EXTRA_LIBS_LDFLAGS)

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Compile .c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .cpp files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean
