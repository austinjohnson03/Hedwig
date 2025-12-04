# Compiler
CC = gcc

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin
OBJ_DIR = obj

# Target executable
TARGET = $(BIN_DIR)/hedwig

# Libraries
LIBS = -lpq
INCLUDES = -I/usr/include/postgresql -I$(INCLUDE_DIR)

# Compilation flags
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -std=c11 -O2 $(INCLUDES)

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: $(BIN_DIR) $(OBJ_DIR) $(TARGET)

# Create bin and obj directories if they don't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean
