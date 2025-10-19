add_library(utilities_lib STATIC utilities.cc)

target_include_directories(utilities_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS
        utilities_lib
        ARCHIVE DESTINATION lib)
