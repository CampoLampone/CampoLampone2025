add_library(controler_pid STATIC pid.c)

target_include_directories(controler_pid PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes)

target_link_libraries(controler_pid PUBLIC pico_stdlib)
