# Ensure build directory exists
mkdir -p build
cd build

# Run cmake if no makefile
if [ ! -f Makefile ]; then
    cmake ..
fi

# build
make
