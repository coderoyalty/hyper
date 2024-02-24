cmake_minimum_required(VERSION 3.10)
project(opengl-cpp)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(third-party/sol2/single/include)
include_directories(include)

link_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(main ${SOURCES} src/glad.c)

target_link_libraries(main glfw3 opengl32)
