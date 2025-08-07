add_library(driver_encoder STATIC encoder.c)

pico_generate_pio_header(driver_encoder ${CMAKE_CURRENT_LIST_DIR}/quadrature_encoder_substep.pio)

target_include_directories(driver_encoder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

target_link_libraries(driver_encoder PUBLIC hardware_gpio hardware_pwm hardware_pio pico_stdlib)
