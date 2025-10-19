add_library(cube_lib STATIC cube.cc)

target_include_directories(cube_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS
        cube_lib
        ARCHIVE DESTINATION lib)

