project(http-client C CXX ASM)

add_executable(
        http-client
        http-client.cpp
)

add_target_compile_definition_if_not_empty(http-client WIFI_STA_SSID)
add_target_compile_definition_if_not_empty(http-client WIFI_STA_PASSWORD)

pico_enable_stdio_usb(http-client 1)
pico_enable_stdio_uart(http-client 0)

pico_add_extra_outputs(http-client)

target_link_libraries(
        http-client
        pico_stdlib
        pico_binary_info
)

add_deploy_target(http-client "${CMAKE_CURRENT_BINARY_DIR}/http-client.uf2")
