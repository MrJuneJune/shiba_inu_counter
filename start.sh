#!/bin/bash

command_exists() {
    command -v "$1" >/dev/null 2>&1
}


# Check if raylib is included
raylib_exists() {
    if [ -f "/usr/local/include/raylib.h" ] || [ -f "/usr/include/raylib.h" ] || [ -f "/opt/homebrew/include/raylib.h" ] || [-f "/home/linuxbrew/.linuxbrew/opt/raylib/include/raylib.h"]; then
        return 0
    else
        return 1
    fi
}

if ! command_exists cmake; then
    echo "Error: cmake is not installed."
    exit 1
fi

if ! command_exists make; then
    echo "Error: make is not installed."
    exit 1
fi

if ! raylib_exists; then
    echo "Error: raylib library is not installed."
    exit 1
fi

echo "All dependencies are installed. Running make..."
make release
