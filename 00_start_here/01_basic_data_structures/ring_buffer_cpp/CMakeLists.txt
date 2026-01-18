
project(ring_buffer_cpp)

file(GLOB_RECURSE srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.c*)

add_library(ring_buffer_cpp STATIC ${srcs})

target_include_directories(ring_buffer_cpp 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ring_buffer_cpp
    PUBLIC C::Logging
)

add_library(CXX::RingBuffer ALIAS ring_buffer_cpp)

add_executable(ring_buffer_cpp_tb ${CMAKE_CURRENT_LIST_DIR}/tb.cpp)

target_link_libraries(ring_buffer_cpp_tb
    PUBLIC C::Logging
    PUBLIC CXX::RingBuffer
)
