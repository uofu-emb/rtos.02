add_executable(blink
    blink.c
    )

pico_set_program_name(blink "test")
pico_set_program_version(blink "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(blink 0)
pico_enable_stdio_usb(blink 1)

# Add the libraries to the build
target_link_libraries(blink PRIVATE
  pico_stdlib
)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(blink PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(blink)

add_custom_target(flash_blink picotool load -f $<TARGET_FILE:blink> DEPENDS blink)
