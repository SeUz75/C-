# !/bin/bash


create_project () {
    touch CMakeLists.txt
    echo "Please number of files"
    read FILES
    touch CMakeLists.txt

    echo "cmake_minimum_required(VERSION 3.10)" >> CMakeLists.txt
    echo "project()" >> CMakeLists.txt
    echo "add_compile_options(-Wall -Wextra -Wpendatic)" >> CMakeLists.txt
    echo "set(CMAKE_CXX_STANDARD 20)" >> CMakeLists.txt
    echo "set(CMAKE_CXX_STANDART_REQUIRED ON)" >> CMakeLists.txt


    for (( i = 0; i < $FILES; i++)) {
        echo "Enter the name of the file "
        read NAME
        mkdir $NAME
        cd $NAME
        touch $NAME.cc
        touch $NAME.h
        touch CMakeLists.txt
        echo "add_executable()" >> CMakeLists.txt
        echo "target_include_directories()" >> CMakeLists.txt
        echo "target_link_libraries()" >> CMakeLists.txt
        echo "install(TARGETS)" >> CMakeLists.txt
        cd ..
        echo "add_subdirectory($NAME)" >> CMakeLists.txt
    }
}

test_app () {
    rm -r build/ install/
    mkdir -p build install/bin install/lib
    cd build
    cmake ..
    cd ..
    cmake -S . -B build -DCMAKE_INSTALL_PREFIX=./install

    cmake --build build

    cmake --install build -v

    ./install/bin/executable -f 30 -w 1920 -h 1080 -s 10 -S 10 -y 50 -u 128 -v 128 -Y 0 -U 0 -V 0 -a 0 -b 128 -c 255 -o output.y4m

    if [ $? -eq 0 ]; then
        echo "Code compiled"
        ffmpeg -i output.y4m output.mp4
    else
        echo "ERROR the code"
    fi

}