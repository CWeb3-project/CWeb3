cmake_minimum_required(VERSION 3.9)
project("server_test" VERSION 1.0.1 DESCRIPTION "simple server test")

file(GLOB_RECURSE my_c_list RELATIVE ${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/*.c")

foreach(file_path ${my_c_list})
    string( REPLACE ".c" "" new_name ${file_path} )
    get_filename_component(filename ${new_name} NAME)
    add_executable( ${filename} ${file_path} )
    target_link_libraries(${filename} "${CMAKE_SOURCE_DIR}/../libCweb3.so")
endforeach()
