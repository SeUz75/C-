cmake_minimum_required(VERSION 3.10)

project(c_threads)

add_compile_options(-Wall -Wextra -Werror -fPIC)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDART_REQUIRED ON)

add_subdirectory(advanced_dir)
add_subdirectory(app)
add_subdirectory(base_dir)
add_subdirectory(data_structure)
add_subdirectory(dispatcher_dir)
add_subdirectory(simple_dir)
