cmake_minimum_required(VERSION 3.5)

project(OpenGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(OpenGL main.cpp)
target_link_libraries(OpenGL -lGLEW -lGL -lX11 -lGLU glfw)
