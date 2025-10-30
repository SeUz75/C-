add_library(c_dispatcher SHARED dispatcher_process.c)

target_include_directories(c_dispatcher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(c_dispatcher c_base_thread)

install(TARGETS
        c_dispatcher
        LIBRARY DESTINATION lib)
