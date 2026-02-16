
project(BufferPool)

file(GLOB_RECURSE srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.c*)

add_library(BufferPool STATIC ${srcs})

target_include_directories(BufferPool 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(BufferPool
    PUBLIC C::Logging
)

add_library(C::BufferPool ALIAS BufferPool)

add_executable(BufferPool_tb ${CMAKE_CURRENT_LIST_DIR}/tb.cpp)

target_link_libraries(BufferPool_tb
    PUBLIC C::Logging
    PUBLIC C::BufferPool
)
