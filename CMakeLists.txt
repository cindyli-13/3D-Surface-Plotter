cmake_minimum_required(VERSION 3.16)
project(3DSurfacePlotter)

set(CMAKE_CXX_STANDARD 14)

find_package(glfw3 3.3 REQUIRED)
add_executable(3DSurfacePlotter main.cpp glad.c Shader.cpp Shader.h SurfacePlotter.cpp SurfacePlotter.h GLProgram.cpp GLProgram.h)
target_link_libraries(3DSurfacePlotter -lGL glfw)