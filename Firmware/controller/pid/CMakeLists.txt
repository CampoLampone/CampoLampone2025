add_library(controller_pid STATIC pid.c)

target_include_directories(controller_pid PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes)

target_link_libraries(controller_pid PUBLIC pico_stdlib)
