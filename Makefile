# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -Wpedantic -std=c11 -Isrc
LDFLAGS :=

# Project structure
SRC_DIR := src
BUILD_DIR := build

# Target
TARGET := $(BUILD_DIR)/bota

# Source and object files
SRCS := $(SRC_DIR)/main.c \
        $(SRC_DIR)/lexer.c \
        $(SRC_DIR)/utils.c

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Cleanup
clean:
	rm -rf $(BUILD_DIR)

# Convenience
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

