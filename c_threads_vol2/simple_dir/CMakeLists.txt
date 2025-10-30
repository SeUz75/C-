add_library(c_simple SHARED simple_process.c)

target_include_directories(c_simple PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(c_simple c_base_thread)

install(TARGETS
        c_simple
        LIBRARY DESTINATION lib)
