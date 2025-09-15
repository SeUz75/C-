add_library ( advanced_process SHARED advanced_process_lib.c)

target_include_directories ( advanced_process PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(advanced_process PUBLIC base_thread)
