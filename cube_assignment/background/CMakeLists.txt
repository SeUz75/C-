add_library(background_lib STATIC background.cc)

target_include_directories(background_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS
        background_lib
        ARCHIVE DESTINATION lib)
