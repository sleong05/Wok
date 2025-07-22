CXX = g++
CXXFLAGS = -Wall -g -O3 -MMD -Iinclude
LDFLAGS =  -lsfml-graphics -lsfml-window -lsfml-system

TARGET = main

# Source and build directories
SRC_DIR = src
BUILD_DIR = build

# All source files
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')

# Turn each src/foo/bar.cpp into build/foo/bar.o
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

# Compile step — create build path and compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(TARGET) $(BUILD_DIR)

# Include dependency files
-include $(DEPS)

.PHONY: all clean

