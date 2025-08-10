add_library(driver_spi STATIC spi.c)

target_include_directories(driver_spi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

pico_generate_pio_header(driver_spi ${CMAKE_CURRENT_LIST_DIR}/spi_read_data.pio)

target_link_libraries(driver_spi PUBLIC pico_stdlib hardware_dma hardware_pio)
