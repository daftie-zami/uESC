#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

echo "Setting up the project in: $PROJECT_ROOT"
cd "$PROJECT_ROOT" || {
    echo "Failed to change to project root directory: $PROJECT_ROOT" >&2
    exit 1
}

echo "Initializing and updating git submodules..."
git submodule update --init --recursive || {
    echo "Failed to initialize submodules." >&2
    exit 1
}

echo "Building libopencm3 for project"
make -C  libopencm3/ || {
    echo "Failed to build libopencm3." >&2
    exit 1
}

echo "All done."
