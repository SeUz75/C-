add_executable(executable app.cc)

target_include_directories(executable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(executable PUBLIC utilities_lib)

install(TARGETS
executable
RUNTIME DESTINATION bin)
