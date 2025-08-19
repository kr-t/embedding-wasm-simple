#!/bin/bash

set -e

BUILD_DIR="$PWD/build"

cmake -S . -B "$BUILD_DIR"
cd "$BUILD_DIR"
make

if [ "$1" == "-r" ]; then
    shift # Remove the -r flag from the arguments
    # Check if the last line of the build output indicates success
    LAST_LINE=$(make | tail -n 1)
    if [[ "$LAST_LINE" == *"[100%] Built target app"* ]]; then
        echo "========================"
        echo ""
        "./app" "$@"
    else
        echo "Build failed or did not complete successfully."
        exit 1
    fi
fi
