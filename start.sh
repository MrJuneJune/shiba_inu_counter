#!/bin/bash

command_exists() {
    command -v "$1" >/dev/null 2>&1
}


if ! command_exists cmake; then
    echo "Error: cmake is not installed."
    exit 1
fi

if ! command_exists make; then
    echo "Error: make is not installed."
    exit 1
fi

echo "All dependencies are installed. Running make..."
make release

echo "Starting the game"
./build/shiba_inu
