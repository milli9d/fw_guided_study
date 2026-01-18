
project(ring_buffer)

file(GLOB_RECURSE srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.c*)

add_library(ring_buffer STATIC ${srcs})

target_include_directories(ring_buffer 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ring_buffer
    PUBLIC C::Logging
)

add_library(C::RingBuffer ALIAS ring_buffer)

add_executable(ring_buffer_tb ${CMAKE_CURRENT_LIST_DIR}/tb.c)

target_link_libraries(ring_buffer_tb
    PUBLIC C::Logging
    PUBLIC C::RingBuffer
)
