cmake_minimum_required(VERSION 3.7)
project(chip8)

set(CMAKE_CXX_STANDARD 14)

set(DISASSEMBLER_SOURCE_FILES disassembler/disassembler.cpp)
add_executable(disassembler ${DISASSEMBLER_SOURCE_FILES})
target_include_directories(disassembler PRIVATE ${CMAKE_SOURCE_DIR}/src)

set(SOURCE_FILES
  src/main.cpp
  src/instructions.hpp
  src/cpu.hpp
  )
add_executable(chip8 ${SOURCE_FILES})

enable_testing()
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)
foreach(testSrc ${TEST_SRCS})
  get_filename_component(testName ${testSrc} NAME_WE)
  add_executable(${testName} ${testSrc})
  target_include_directories(${testName}
    PRIVATE ${BOOST_INCLUDE_DIRS}
    PRIVATE ${CMAKE_SOURCE_DIR}/src)
  target_link_libraries(${testName} ${Boost_LIBRARIES})
  add_test(NAME ${testName} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test/${testName})
endforeach(testSrc)
