cmake_minimum_required(VERSION 3.7)
project(chip8)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/instructions.cpp src/cpu.hpp)
add_executable(chip8 ${SOURCE_FILES})
