
project(CPP_freelist)

file(GLOB_RECURSE srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.c*)

add_library(CPP_freelist STATIC ${srcs})

target_include_directories(CPP_freelist 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(CPP_freelist
    PUBLIC C::Logging
)

add_library(CXX::CPP_FreeList ALIAS CPP_freelist)

add_executable(CPP_freelist_tb 
    ${CMAKE_CURRENT_LIST_DIR}/tb.cpp
)

target_link_libraries(CPP_freelist_tb
    PUBLIC C::Logging
    PUBLIC CXX::CPP_FreeList
)
