cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)
# setup board type (alternative - pico_w)
set(PICO_BOARD pico)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(bare-metal-display C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(bare-metal-display)
add_subdirectory(example-fonts)
add_subdirectory(example-lines)
add_subdirectory(example-images)
add_subdirectory(example-circles)

