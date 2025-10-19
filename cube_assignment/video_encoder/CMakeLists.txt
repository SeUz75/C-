add_library(video_encoder_lib STATIC video_encoder.cc)

target_include_directories(video_encoder_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS
        video_encoder_lib
        ARCHIVE DESTINATION lib)
