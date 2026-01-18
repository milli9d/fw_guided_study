project(sorting)

file(GLOB_RECURSE srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.c*)

add_library(sorting STATIC ${srcs})

target_include_directories(sorting 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(sorting
    PUBLIC C::Logging
)

add_library(C::Sorting ALIAS sorting)

add_executable(sorting_tb ${CMAKE_CURRENT_LIST_DIR}/tb.cpp)

target_link_libraries(sorting_tb
    PUBLIC C::Logging
    PUBLIC C::Sorting
)
