# Compiler
CC = clang

# C Pre-Processor flags
CPP_FLAGS = -Iinclude

# Compiler flags
CFLAGS_DEBUG = -g -O0 -Wall -Wextra -Wpedantic -fsanitize=address,undefined
CFLAGS_RELEASE = -O2 -DNDEBUG -flto
LDFLAGS = -lSDL3

# Default vm file to run
# Usage: make run FILE=BasicTest.vm
FILE ?= BasicTest.vm

# Project structure
BUILD_DIR = bin
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)

# Executable names
TARGET_DEBUG = $(BUILD_DIR)/vm_translator_debug
TARGET_RELEASE = $(BUILD_DIR)/vm_translator

# Phony targets
.PHONY: all debug release clean run-debug run-release

# Default target
all: debug

# Build rules
debug: $(TARGET_DEBUG)
release: $(TARGET_RELEASE)

# Generic rule to build targets from source files
$(TARGET_DEBUG): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPP_FLAGS) $(CFLAGS_DEBUG) $^ -o $@ $(LDFLAGS)

$(TARGET_RELEASE): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPP_FLAGS) $(CFLAGS_RELEASE) $^ -o $@ $(LDFLAGS)

# Clean rule
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)

# Run rules
run: run-release

run-debug: debug
	@echo "--- Running Debug Build with FILE: $(FILE) ---"
	./$(TARGET_DEBUG) $(FILE)

run-release: release
	@echo "--- Running Release Build with FILE: $(FILE) ---"
	./$(TARGET_RELEASE) $(FILE)