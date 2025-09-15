add_library(dispatcher
  dispatcher_lib.c
)

target_include_directories(dispatcher
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(dispatcher
    PUBLIC
    advanced_process
    base_thread
    simple_process
)
