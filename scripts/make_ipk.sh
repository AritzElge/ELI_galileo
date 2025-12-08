#!/bin/bash

# --- Path Configuration ---
# Path to the main Buildroot directory
BUILDROOT_DIR="./galileo/buildroot"
# Path to your external tree (relative to the directory where you run this script)
EXTERNAL_DIR="./src"
# --------------------------

echo "Starting Buildroot compilation with BR2_EXTERNAL..."

# Navigate into the Buildroot directory
cd "$BUILDROOT_DIR" || exit

# Clean and compile, passing the external path relative to *this* directory (galileo/buildroot)
# We assume $EXTERNAL_DIR is one level up (../../) and parallel to galileo/buildroot
make clean
make BR2_EXTERNAL="../../$EXTERNAL_DIR" all

# Return to the original directory
cd -

echo "Process completed."
