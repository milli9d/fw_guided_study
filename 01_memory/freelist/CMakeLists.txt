
project(freelist)

file(GLOB_RECURSE srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.c*)

add_library(freelist STATIC ${srcs})

target_include_directories(freelist 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(freelist
    PUBLIC C::Logging
)

add_library(C::FreeList ALIAS freelist)

add_executable(freelist_tb ${CMAKE_CURRENT_LIST_DIR}/tb.cpp)

target_link_libraries(freelist_tb
    PUBLIC C::Logging
    PUBLIC C::FreeList
)
