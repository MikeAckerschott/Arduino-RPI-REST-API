cmake_minimum_required(VERSION 3.10)

# Project name
project(PinFlipProject)

# Find the bcm2835 library
find_library(BCM2835_LIB bcm2835)

# If the bcm2835 library is not found, terminate the build with an error
if (NOT BCM2835_LIB)
    message(FATAL_ERROR "Could not find bcm2835 library")
endif()

# Add the executable
add_executable(pinflip pinflip.c)

# Link the bcm2835 library to your executable
target_link_libraries(pinflip ${BCM2835_LIB})

# Set the C standard (optional, but good practice)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)