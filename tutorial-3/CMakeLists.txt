cmake_minimum_required(VERSION 3.16)

project(opengl_tutorial_3)

set(INCLUDE_DIRS /home/sonul/Documents/github/opengl/glad/include)
set(CMAKE_PREFIX_PATH /home/sonul/Documents/github/opengl/glfw/lib/cmake/)
find_package(glfw3 REQUIRED)

add_executable(opengl_tutorial_3 main.cpp glad.c)
target_link_libraries(opengl_tutorial_3 glfw)
target_include_directories(opengl_tutorial_3 PUBLIC ${INCLUDE_DIRS})
