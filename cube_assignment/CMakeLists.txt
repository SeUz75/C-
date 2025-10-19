cmake_minimum_required(VERSION 3.10)

project(video_generator)

add_compile_options(-Wall -Wextra -Wpedantic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(background)
add_subdirectory(cube)
add_subdirectory(utilities)
add_subdirectory(video_encoder)
add_subdirectory(app)
