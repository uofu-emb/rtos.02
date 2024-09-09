add_executable(mytest test.c)

target_link_libraries(mytest PRIVATE
  pico_stdlib
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  unity
)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    target_link_libraries(mytest PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Renode doesn't support the divider accellerator, disable it.
target_compile_definitions(mytest
    PRIVATE PICO_EMULATE_DIVIDER=1
)

pico_enable_stdio_uart(mytest 1)
pico_set_binary_type(mytest no_flash)
find_program(RENODE renode
# This is extra stuff so it can find the mac installation paths.
 NAMES macos_run.command Renode.exe
 PATHS /Applications/Renode.app/Contents/MacOS)

set(RENODE_FLAGS
  --disable-xwt
  --port -2
  --pid-file renode.pid
  --console
  )

add_test(NAME simulate_mytest COMMAND
    ${RENODE}
     ${RENODE_FLAGS}
    -e "$ELF=@$<TARGET_FILE:mytest>; include @${CMAKE_SOURCE_DIR}/test/simulate.resc; start"
    )
