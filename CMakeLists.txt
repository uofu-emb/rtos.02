cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# This sets up some environment variables needed by VSCode
include(vscode.cmake)

# This sets the variant of the board. We are using the pico_w
set(PICO_BOARD pico_w CACHE STRING "Board type")

# This imports the Pico SDK (must be included before the project)
include(lib/pico-sdk/pico_sdk_init.cmake)

# This imports the library configuration for FreeRTOS
include(lib/freertos/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# This sets up the unity library
include(unity.cmake)

# This defines your project name, you can name it anything you want.
project(myproject C CXX ASM)

# These are some macros from the pico SDK that do some setup.
pico_sdk_init()

# This sets a preprocessor value for use in our code.
add_compile_definitions(
    configNUMBER_OF_CORES=1
    )

# This is where your project header files are located.
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# To keep things organized, the source code configuration is inside the src directory
add_subdirectory(src)

include(CTest)

# Similarly, the test configuration is inside the test directory.
add_subdirectory(test)
