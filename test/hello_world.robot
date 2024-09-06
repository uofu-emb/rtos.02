*** Test Cases ***
hello_world on rpi_pico
    ${x}=                       Execute Command         include @${CURDIR}/hello_world.resc
    Create Terminal Tester      sysbus.uart0    timeout=5    defaultPauseEmulation=true

    Wait For Line On Uart       *** Booting Zephyr OS build 0237d375de5f ***
    Wait For Line On Uart       Hello World! rpi_pico