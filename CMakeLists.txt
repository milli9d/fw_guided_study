# set the minimum version of CMake
cmake_minimum_required(VERSION 3.30.1)

# set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
# set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

# Use system ar instead of LLVM ar to avoid archive corruption
set(CMAKE_AR "/usr/bin/ar")
set(CMAKE_RANLIB "/usr/bin/ranlib")

# set the project name
project(interview_practice)

# Enable CTest integration
include(CTest)
enable_testing()

# set the C and C++ standards
# set(CMAKE_C_STANDARD 26)
set(CMAKE_CXX_STANDARD 26)

# set(CMAKE_CXX_SCAN_FOR_MODULES TRUE)
# set(CMAKE_CXX_COMPILER_IMPORT_STD TRUE)

# set the output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/out/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/out/bin)

# set the flags
set(CMAKE_CXX_FLAGS "-g -Ofast -Wall -Wextra -Wunused")
set(CMAKE_C_FLAGS "-g -Ofast -Wall -Wextra -Wunused")

# add all the subdirectories
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/logging)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/00_start_here)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/01_memory)


