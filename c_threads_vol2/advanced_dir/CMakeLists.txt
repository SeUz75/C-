add_library(c_advanced SHARED advanced_process.c)

target_include_directories(c_advanced PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(c_advanced c_base_thread)

install(TARGETS
        c_advanced
        LIBRARY DESTINATION lib)
