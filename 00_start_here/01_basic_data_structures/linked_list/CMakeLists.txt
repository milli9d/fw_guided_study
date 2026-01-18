
project(linked_list)

file(GLOB_RECURSE srcs ${CMAKE_CURRENT_LIST_DIR}/src/*.c*)

add_library(linked_list STATIC ${srcs})

target_include_directories(linked_list 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(linked_list
    PUBLIC C::Logging
)

add_library(C::LinkedList ALIAS linked_list)

add_executable(linked_list_tb ${CMAKE_CURRENT_LIST_DIR}/tb.c)

target_link_libraries(linked_list_tb
    PUBLIC C::Logging
    PUBLIC C::LinkedList
)
