# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Power_Management_HAT C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

#添加编译子目录
add_subdirectory(./lib/Config)
add_subdirectory(./lib/GUI)
add_subdirectory(./lib/LCD)
add_subdirectory(./lib/Fonts)
add_subdirectory(./lib/INA219)
add_subdirectory(./lib/QMI8658)
add_subdirectory(./lib/MP28167)
add_subdirectory(./lib/PCF85063A)
add_subdirectory(./lib/Power_Management_Lib)
add_subdirectory(./examples)
#添加头文件目录
include_directories(./lib/Config)
include_directories(./lib/GUI)
include_directories(./lib/LCD)
include_directories(./lib/INA219)
include_directories(./lib/QMI8658)
include_directories(./lib/MP28167)
include_directories(./lib/PCF85063A)
include_directories(./lib/Power_Management_Lib)
include_directories(./examples)

# Add executable. Default name is the project name, version 0.1

add_executable(Power_Management_HAT Power_Management_HAT.c )

pico_set_program_name(Power_Management_HAT "Power_Management_HAT")
pico_set_program_version(Power_Management_HAT "0.1")

pico_enable_stdio_uart(Power_Management_HAT 1)
pico_enable_stdio_usb(Power_Management_HAT 1)

pico_add_extra_outputs(Power_Management_HAT)

# Add the standard library to the build
target_link_libraries(Power_Management_HAT examples LCD INA219 PCF85063A Power_Management_Lib QMI8658 GUI Fonts Config pico_stdlib hardware_i2c )



