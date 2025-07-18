#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REQUIREMENTS_FILE="$SCRIPT_DIR/requirements.txt"

echo "Initializing and updating git submodules..."
git submodule update --init --recursive || {
    echo "Failed to initialize submodules." >&2
    exit 1
}

echo "Checking Python installation..."
if ! command -v python3 >/dev/null 2>&1; then
    echo "Python 3 is not installed." >&2
    exit 1
fi
