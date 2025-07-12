add_library(driver_motor STATIC motor.c)

target_include_directories(driver_motor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

target_link_libraries(driver_motor PUBLIC hardware_gpio hardware_pwm pico_stdlib)
