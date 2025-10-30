add_library(c_base_thread STATIC base_thread.c)

target_include_directories(c_base_thread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(c_base_thread c_data_structure)
