cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/boards)

set(PICO_BOARD board_lampone)

include(pico-sdk/pico_sdk_init.cmake)
include(pico_sdk_import.cmake)

project(lampone)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
pico_sdk_init()

add_executable(${PROJECT_NAME} main.c control_speed.c)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_subdirectory(driver)
add_subdirectory(controler)

target_link_libraries(${PROJECT_NAME} pico_stdlib pico_stdio_usb driver_motor driver_encoder controler_pid)

pico_add_extra_outputs(${PROJECT_NAME})
