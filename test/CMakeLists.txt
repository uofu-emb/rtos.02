add_executable(mytest test.c unity_config.c)# ${CMAKE_SOURCE_DIR}/src/hello_freertos.c)

target_link_libraries(mytest PRIVATE
  pico_stdlib
  # pico_multicore
  # pico_async_context_freertos
  # FreeRTOS-Kernel-Heap4
  unity
)
# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(mytest PRIVATE
        pico_cyw43_arch_none
    )
endif()

# pico_set_binary_type(mytest no_flash)

pico_enable_stdio_uart(mytest 1)

find_program(RENODE renode)

set(RENODE_FLAGS
  --disable-xwt
  --port -2
  --pid-file renode.pid
  --console
  )

add_test(NAME runmytest COMMAND
    ${RENODE}
     ${RENODE_FLAGS}
    -e "$elf=@$<TARGET_FILE:mytest>; $cwd=@${CMAKE_SOURCE_DIR}/output; include @${CMAKE_SOURCE_DIR}/test/test.resc"
    )
