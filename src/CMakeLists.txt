cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake ../tinyusb/src/tusb.h)

project(midi-keyboard C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(midi-keyboard
    midi-keyboard.c
    usb_descriptors.c
)

pico_enable_stdio_uart(midi-keyboard 1)
pico_enable_stdio_usb(midi-keyboard 1)

pico_add_extra_outputs(midi-keyboard)

target_include_directories(midi-keyboard PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(midi-keyboard
    pico_stdlib
    pico_multicore
    tinyusb_device
    tinyusb_board
)