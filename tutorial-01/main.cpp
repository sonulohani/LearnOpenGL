#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <soil2/SOIL2.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
};

Vertex vertices[] = {
    // Position                     // Color                            // Texcoords
    glm::vec3(-0.5f, 0.5f, 0.f),      glm::vec3(1.f, 0.f, 0.f),         glm::vec2(0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.f),   glm::vec3(0.f, 1.f, 0.f),           glm::vec2(0.f, 0.f),
    glm::vec3(0.5f, -0.5f, 0.f),    glm::vec3(0.f, 0.f, 1.f),           glm::vec2(1.f, 0.f),

    glm::vec3(0.5f, 0.5f, 0.f),    glm::vec3(1.f, 1.f, 0.f),             glm::vec2(1.f, 1.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {
    0, 1, 2,
    0, 2, 3
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

void updateInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
    glViewport(0, 0, fbW, fbH);
}

bool loadShaders(GLuint& program) {
    char infoLog[512];
    GLint success;
    
    std::string temp = "";
    std::string src = "";

    std::ifstream in_file;

    // Vertex shader
    in_file.open("vertex_core.glsl");
    if(in_file.is_open()) {
        while(std::getline(in_file, temp)) {
            src += temp + "\n";
        }
    } else {
        std::cout << "ERROR::LOADSHADERS::COULD NOT OPEN VERTEX FILE" << '\n';
        return false;
    }

    in_file.close();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertSrc = src.c_str();
    glShaderSource(vertexShader, 1, &vertSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::LOADSHADERS::COULD NOT COMPILE VERTEX FILE" << '\n';
        std::cout << infoLog << '\n';

        return false;
    }

    temp = {};
    src = {};

    // Fragment shader
    in_file.open("fragment_core.glsl");
    if(in_file.is_open()) {
        while(std::getline(in_file, temp)) {
            src += temp + "\n";
        }
    } else {
        std::cout << "ERROR::LOADSHADERS::COULD NOT OPEN FRAGMENT FILE" << '\n';
        return false;
    }

    in_file.close();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragSrc = src.c_str();
    glShaderSource(fragmentShader, 1, &fragSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::LOADSHADERS::COULD NOT COMPILE FRAGMENT FILE" << '\n';
        std::cout << infoLog << '\n';

        return false;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::LOADSHADERS::COULD NOT LINK PROGRAM" << '\n';
        std::cout << infoLog << '\n';

        return false;
    }

    glUseProgram(0);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

int main()
{
    glfwInit();

    const int WINDOW_WIDTH=640;
    const int WINDOW_HEIGHT=480;
    int framebufferWidth = 0;
    int framebufferHeight = 0;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Youtube Tutorial", nullptr, nullptr);

    // glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    glfwMakeContextCurrent(window);

    glewExperimental = true;

    if(glewInit() != GLEW_OK) {
        std::cout << "Error: main.cpp, glewInit failed";
        glfwTerminate();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    GLuint core_program;
    if(!loadShaders(core_program)) {
        glfwTerminate();
    }

    GLuint VAO;
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLuint attribLoc = glGetAttribLocation(core_program, "vertex_position");
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(attribLoc);

    attribLoc = glGetAttribLocation(core_program, "vertex_color");
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(attribLoc);

    attribLoc = glGetAttribLocation(core_program, "vertex_texcoord");
    glVertexAttribPointer(attribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(attribLoc);

    glBindVertexArray(0);

    int image_width = {};
    int image_height = {};

    unsigned char* image = SOIL_load_image("Images/cat.png", &image_width, &image_height, nullptr, SOIL_LOAD_RGBA);
    GLuint texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    
    if(image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else {
        std::cout << "ERROR: TEXTURE LOADING FAILED" << '\n';
        glfwTerminate();
        return -1;
    }

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    int image_width1 = {};
    int image_height1 = {};

    unsigned char* image1 = SOIL_load_image("Images/container.png", &image_width1, &image_height1, nullptr, SOIL_LOAD_RGBA);
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    if(image1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else {
        std::cout << "ERROR: TEXTURE LOADING FAILED" << '\n';
        glfwTerminate();
        return -1;
    }

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image1);

    glm::mat4 ModelMatrix{1.f};
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
    // ModelMatrix = glm::rotate(ModelMatrix, glm::radians(50.f), glm::vec3(1.f, 0.f, 0.f));
    // ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
    // ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));

    glm::vec3 camPosition(0.f, 0.f, 1.f);
    glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 camFront = glm::vec3(0.f, 0.f, -1.f);
    glm::mat4 ViewMatrix(1.f);
    ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

    float fov = 90.f;
    float nearPlane = 0.1f;
    float farPlane = 1000.f;
    glm::mat4 ProjectionMatrix(1.f);
    ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth)/framebufferHeight, nearPlane, farPlane);


    glUseProgram(core_program);
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    glUseProgram(0);

    while(!glfwWindowShouldClose(window)) {
        updateInput(window);

        glfwPollEvents();
        glClearColor(0.f,0.f,0.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glUseProgram(core_program);

        glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        glUniform1i(glGetUniformLocation(core_program, "texture1"), 1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
        // ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5.f), glm::vec3(1.f, 0.f, 0.f));
        // ModelMatrix = glm::rotate(ModelMatrix, glm::radians(2.f), glm::vec3(0.f, 1.f, 0.f));
        // ModelMatrix = glm::rotate(ModelMatrix, glm::radians(1.f), glm::vec3(0.f, 0.f, 1.f));
        // ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.001f));

        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
        ProjectionMatrix = glm::mat4{1.f};
        ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth)/framebufferHeight, nearPlane, farPlane);

        glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(core_program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glFlush();

        glBindVertexArray(0);
        glUseProgram(0);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    glDeleteProgram(core_program);
}