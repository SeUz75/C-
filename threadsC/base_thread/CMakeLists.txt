add_library ( base_thread STATIC base_thread_lib.c data_structure.c)

target_include_directories ( base_thread PUBLIC ${CMAKE_CURRENT_LIST_DIR})

