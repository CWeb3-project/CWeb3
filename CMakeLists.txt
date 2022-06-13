Project("Cweb3")

set(SRCDir "${CMAKE_SOURCE_DIR}/src")
set(HDRDir "${CMAKE_SOURCE_DIR}/include")

file (GLOB SRCFiles ${SRCDir}/*.c)
file (GLOB HDRFiles ${HDRDir}/*.h)

add_executable (${PROJECT_NAME} ${SRCFiles} ${HDRFiles})