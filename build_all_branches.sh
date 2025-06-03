#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status
set -o pipefail  # Catch errors in pipelines

PROJECT_NAME="ParallelCGP"  # Name of the executable (without .exe for Unix)
BUILD_COMMAND="cmake -S ../.. -B . -DCMAKE_CXX_COMPILER=g++"
MAKE_COMMAND="make"

BUILD_FOLDER="unix_build"  # Folder where the build will be done

if [ -d "$BUILD_FOLDER" ]; then
    rm -rf "$BUILD_FOLDER"
fi
mkdir "$BUILD_FOLDER"
cd "$BUILD_FOLDER"

# Fetch all branches
echo "Fetching all branches..."
git fetch --all

# Get a list of all branches
for BRANCH in $(git branch -r | grep -v "HEAD" | sed 's/origin\///'); do
    echo "=========================================="
    echo "Checking out branch: $BRANCH"
    echo "=========================================="
    git checkout "$BRANCH" || exit 1

    # Create a separate folder for the branch
    BUILD_DIR="$BRANCH"
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
    fi
    mkdir "$BUILD_DIR"
    cd "$BUILD_DIR"

    # Run the build command
    echo "Building branch: $BRANCH"
    eval "$BUILD_COMMAND"
    eval "$MAKE_COMMAND" || { echo "Build failed for branch: $BRANCH"; cd ..; exit 1; }

    # Keep only the executable and delete other files and subdirectories
    echo "Cleaning up build directory: $BUILD_DIR"
    find . -type f ! -name "$PROJECT_NAME" -delete
    find . -type d ! -name "." -exec rm -rf {} +

    cd ..
done

echo "=========================================="
echo "All branches have been built successfully!"
echo "=========================================="