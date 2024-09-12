cmake_minimum_required(VERSION 3.13)

# local config (ssid, password, etc)
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/config.cmake)
    include(config.cmake)
endif ()

function(add_target_compile_definition_if_not_empty TARGET NAME)
    if (NOT "${${NAME}}" STREQUAL "")
        target_compile_definitions(${TARGET} PRIVATE ${NAME}=\"${${NAME}}\")
    elseif (NOT "$ENV{${NAME}}" STREQUAL "")
        target_compile_definitions(${TARGET} PRIVATE ${NAME}=\"$ENV{${NAME}}\")
    endif ()
endfunction()


# specified board definitions
list(APPEND PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/src/boards)

set(PICO_BOARD rp2040_esp8285)

include(pico_sdk_import.cmake)
include(scripts/functions.cmake)

project(pico-esp8285-test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_subdirectory(src/examples/blink)
add_subdirectory(src/examples/http-client)
