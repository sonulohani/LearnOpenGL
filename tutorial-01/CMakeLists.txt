cmake_minimum_required(VERSION 3.22)

project(tutorial-01)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(soil2 REQUIRED)

add_executable(tutorial-1 main.cpp)
target_link_libraries(tutorial-1 PRIVATE glfw GLEW OpenGL::GL glm::glm soil2::soil2)

add_custom_command(
    TARGET tutorial-1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/fragment_core.glsl
            fragment_core.glsl
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/vertex_core.glsl
            vertex_core.glsl
    COMMENT "Copying shaders to build directory"
)

add_custom_command(
    TARGET tutorial-1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/Images/cat.png
            Images/cat.png
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/Images/container.png
            Images/container.png
    COMMENT "Copying images to build directory"
)