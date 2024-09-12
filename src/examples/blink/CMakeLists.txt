project(blink C CXX ASM)

add_executable(
        blink
        blink.cpp
)

pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)

pico_add_extra_outputs(blink)

target_link_libraries(
        blink
        pico_stdlib
        pico_binary_info
)

add_deploy_target(blink "${CMAKE_CURRENT_BINARY_DIR}/blink.uf2")
