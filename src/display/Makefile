# ====================================================================
# Makefile for ST7920 C++ Library for Intel Galileo Gen 2
# ====================================================================

# Define the path to your cross-compiler base.
# Assumes the buildroot folder is one directory level up (../buildroot).
BUILDROOT_OUTPUT ?= ../../galileo/buildroot/output

# Use find to locate the C++ compiler (g++) within the Buildroot output path.
CROSS_COMPILE := $(shell find $(BUILDROOT_OUTPUT)/host/bin -name "*i586-linux-g++" | head -n 1)

# Define the staging directory where mraa headers and libs are installed
STAGING_DIR := $(BUILDROOT_OUTPUT)/host/i586-buildroot-linux-uclibc/sysroot

# Inclusion and Library Flags (where to look for .h and .a/.so files)
# Includes the standard MRAA path and the local headers folder inc/
MRAA_FLAGS := -I$(STAGING_DIR)/usr/include -L$(STAGING_DIR)/usr/lib -Iinc/

# Linker Flags (LDFLAGS): 
# Link against the mraa library
LDFLAGS := -lmraa

# Compiler Flags for C++ (CXXFLAGS - Note the X)
# -std=c++11 is necessary for <chrono> and <thread> standards support.
CXXFLAGS := -Wall -Wextra -std=c++11 -O2 $(MRAA_FLAGS)

# Define the source files (.cpp) using wildcard in the src/ folder
SRCS := $(wildcard src/*.cpp)

# Define the object files (.o) that will be generated in an 'obj' folder
# Transforms src/file.cpp into obj/file.o
OBJS := $(patsubst src/%.cpp, obj/%.o, $(SRCS))

# Define the name of the final executable target
TARGET := lcd_test_app

# --------------------------------------------------
# Build Rules 
# --------------------------------------------------

# The 'all' target depends on the obj folder and the final executable
all: obj $(TARGET)

# Rule to create the obj directory
obj:
	mkdir -p obj

# Rule to link all .o files into the final executable
$(TARGET): $(OBJS)
	@echo "----------------------------------------------------"
	@echo "Linking C++ '$(TARGET)' using:"
	@echo $(CROSS_COMPILE)
	@echo "----------------------------------------------------"
	# GCC/G++ automatically uses CXXFLAGS and LDFLAGS during the linking phase if not specified otherwise
	$(CROSS_COMPILE) $(OBJS) -o $@ $(MRAA_FLAGS) $(LDFLAGS)

# Rule to compile an individual .cpp file into a .o file
# $< is the first prerequisite (src/%.cpp)
# $@ is the target (obj/%.o)
obj/%.o: src/%.cpp
	@echo "Compiling $< -> $@"
	# Use -c to compile without linking
	$(CROSS_COMPILE) $(CXXFLAGS) -c $< -o $@

# Rule to clean up generated files
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(TARGET) obj/

.PHONY: all clean obj
