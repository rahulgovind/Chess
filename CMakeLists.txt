cmake_minimum_required (VERSION 2.8)

project(Chess)

link_directories("${CMAKE_SOURCE_DIR}/lib")
include_directories("${CMAKE_SOURCE_DIR}/include")

set(CMAKE_BINARY_DIR "${CMAKE_BINARY_DIR}/bin" )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS "-std=c++0x -static-libgcc -static-libstdc++ ${CMAKE_CXX_FLAGS}")


file(COPY "${CMAKE_SOURCE_DIR}/images" DESTINATION "${CMAKE_BINARY_DIR}" )
file(COPY "${CMAKE_SOURCE_DIR}/dlls/glfw3.dll" DESTINATION "${CMAKE_BINARY_DIR}" )

add_executable(
Chess
main.cpp
Board.cpp
Engine.cpp
Engine_AI.cpp
image_loader.cpp
gl.cpp
)

target_link_libraries(Chess glew32s.lib glew32.lib libopengl32.lib libglfw3dll.a libglfw3.a libgdi32.a)
