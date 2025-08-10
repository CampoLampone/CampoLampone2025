add_library(controller_speed STATIC speed_controller.c)

target_include_directories(controller_speed PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes)

target_link_libraries(controller_speed PUBLIC pico_stdlib driver_encoder driver_motor controller_pid)
