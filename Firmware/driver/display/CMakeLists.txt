add_library(driver_display STATIC display.c)

target_include_directories(driver_display PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

target_link_libraries(driver_display PUBLIC hardware_gpio hardware_i2c pico_stdlib)
