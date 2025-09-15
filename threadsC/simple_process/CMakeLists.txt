add_library ( simple_process SHARED simple_process_lib.c)

target_include_directories ( simple_process PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries( simple_process PUBLIC base_thread)