add_library(background_lib STATIC background.cc)

target_include_directories(background_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(background_lib PUBLIC cube_lib)

install(TARGETS
        background_lib
        ARCHIVE DESTINATION lib)
