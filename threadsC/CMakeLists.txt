cmake_minimum_required(VERSION 3.10)

project (c_threads)

add_compile_options(-Wall -Wextra -Wpedantic -fPIC)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(advanced_process)
add_subdirectory(base_thread)
add_subdirectory(simple_process)
add_subdirectory(dispatcher)
add_subdirectory(app)


install ( TARGETS
        c_threads
        base_thread
        simple_process
        advanced_process
        dispatcher
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)

