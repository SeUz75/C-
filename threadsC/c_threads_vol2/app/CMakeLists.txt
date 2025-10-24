add_executable(c_threads app.c)

target_include_directories(c_threads PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(c_threads c_dispatcher)

install(TARGETS 
        c_threads
        RUNTIME DESTINATION bin)
