
project(linked_list_cpp)

file(GLOB_RECURSE srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.c*)

add_library(linked_list_cpp STATIC ${srcs})

target_include_directories(linked_list_cpp 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(linked_list_cpp
    PUBLIC C::Logging
)

add_library(CXX::LinkedList ALIAS linked_list_cpp)

add_executable(linked_list_cpp_tb ${CMAKE_CURRENT_LIST_DIR}/tb.cpp)

target_link_libraries(linked_list_cpp_tb
    PUBLIC C::Logging
    PUBLIC CXX::LinkedList
)
