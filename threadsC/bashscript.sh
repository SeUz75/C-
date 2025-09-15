rm -rf build/ install/
mkdir -p build install/lib install/bin

# configure (sets install prefix)
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=./install

# build
cmake --build build

# install to ./install
cmake --install build
