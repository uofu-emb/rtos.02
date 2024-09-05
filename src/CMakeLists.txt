# List your C files here.
add_executable(thething
    hello_freertos.c
    )

pico_set_program_name(thething "test")
pico_set_program_version(thething "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(thething 1)
pico_enable_stdio_usb(thething 0)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(thething PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Add the standard library to the build
target_link_libraries(thething PRIVATE
    pico_async_context_freertos
    FreeRTOS-Kernel-Heap4
    pico_stdlib
    )

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(thething)
