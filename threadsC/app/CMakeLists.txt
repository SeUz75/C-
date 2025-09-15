add_executable ( c_threads app.c)

target_link_libraries(c_threads PRIVATE base_thread)

target_link_libraries( c_threads PRIVATE base_thread simple_process)
