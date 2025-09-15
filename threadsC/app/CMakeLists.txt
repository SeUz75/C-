add_executable ( c_threads app.c)

target_link_libraries( c_threads PRIVATE dispatcher)
