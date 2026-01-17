cmake_minimum_required(VERSION 3.25)

project(raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD             23)
set(CMAKE_CXX_STANDARD_REQUIRED    ON)
set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

include(CTest)

# clang-tidy issues
if(APPLE)
  execute_process(
    COMMAND xcrun --sdk macosx --show-sdk-path
    OUTPUT_VARIABLE CMAKE_OSX_SYSROOT
  )
  string(STRIP "${CMAKE_OSX_SYSROOT}" CMAKE_OSX_SYSROOT)
endif()

find_package(Catch REQUIRED)
include(cmake/TestSolution.cmake)

find_package(PNG REQUIRED)

add_subdirectory(libs)
add_subdirectory(tests)
