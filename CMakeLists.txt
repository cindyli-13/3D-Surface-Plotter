cmake_minimum_required(VERSION 3.16)
project(3DSurfacePlotter)

set(CMAKE_CXX_STANDARD 14)

find_package(glfw3 3.3 REQUIRED)
add_executable(3DSurfacePlotter src/main.cpp
                                src/glad.c
                                src/Shader.cpp
                                src/SurfacePlotter.cpp
                                src/GLProgram.cpp
                                src/Camera.cpp)
target_link_libraries(3DSurfacePlotter -lGL glfw)