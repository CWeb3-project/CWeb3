cmake_minimum_required(VERSION 3.9)
project("Cweb3" VERSION 1.0.1 DESCRIPTION "A lightweight and fast web socket")

set(SRCDir "${CMAKE_SOURCE_DIR}/src")
set(HDRDir "${CMAKE_SOURCE_DIR}/include")

file (GLOB SRCFiles ${SRCDir}/*.c)
file (GLOB HDRFiles ${HDRDir}/*.h)

add_library(${PROJECT_NAME} SHARED
    ${SRCFiles} ${HDRFiles}
)
