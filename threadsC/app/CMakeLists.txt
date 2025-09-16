add_executable ( c_threads app.c)

target_include_directories ( simple_process PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries( c_threads PUBLIC dispatcher)
