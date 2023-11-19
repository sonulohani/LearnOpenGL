#include <GL/glew.h>
#include <fstream>
#include <functional>
#include <iostream>

#include "meglwindow.h"

using glGetivFunc = std::function<void(GLuint, GLenum, GLint *)>;
using glGetInfoLogFunc =
    std::function<void(GLuint, GLsizei, GLsizei *, GLchar *)>;
using glDeleteFunc = std::function<void(GLuint)>;

std::string readShaderCode(const std::string &filePath) {
  std::string shaderCode;
  std::ifstream file(filePath);

  if (file.is_open()) {
    std::string line;
    while (getline(file, line)) {
      shaderCode += line + "\n";
    }
    file.close();
    return shaderCode;
  } else {
    std::cerr << "Error: Unable to open shader code file at '" << filePath
              << "'." << std::endl;
    return "";
  }
}

bool checkStatus(GLuint objectId, glGetivFunc ivFunc,
                 glGetInfoLogFunc infoLogFunc, glDeleteFunc deleteFunc) {
  GLint success;
  ivFunc(objectId, GL_COMPILE_STATUS, &success);

  if (success == GL_FALSE) {
    GLint maxLength;
    ivFunc(objectId, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> errorLog(maxLength);
    infoLogFunc(objectId, maxLength, &maxLength, &errorLog[0]);

    // Print the error log
    std::cerr << "Shader/Program compilation/linking failed:\n"
              << &errorLog[0] << std::endl;

    // Delete the shader if it failed to compile
    deleteFunc(objectId);
    return false;
  }
  return true;
}

void sendDataToOpenGL() {
  GLfloat verts[] = {
      0.f,  0.f,  1.f, 0.f, 0.f,

      1.f,  1.f,  0.f, 1.f, 0.f,

      -1.f, 1.f,  0.f, 0.f, 1.f,

      -1.f, -1.f, 1.f, 0.f, 0.f,

      1.f,  -1.f, 1.f, 0.f, 0.f,
  };
  GLuint vertexBufferID;
  glGenBuffers(1, &vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (void *)(sizeof(float) * 2));

  GLushort indices[] = {0, 1, 2, 0, 3, 4};
  GLuint indexBufferID;
  glGenBuffers(1, &indexBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
}

void installShaders() {
  GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  const char *adapter[1];
  std::string temp = readShaderCode("shader/VertexShaderCode.vert");
  adapter[0] = temp.c_str();
  glShaderSource(vertexShaderID, 1, adapter, 0);
  temp = readShaderCode("shader/FragmentShaderCode.frag");
  adapter[0] = temp.c_str();
  glShaderSource(fragmentShaderID, 1, adapter, 0);

  glCompileShader(vertexShaderID);
  if (!checkStatus(vertexShaderID, glGetShaderiv, glGetShaderInfoLog,
                   glDeleteShader)) {
    return;
  }

  glCompileShader(fragmentShaderID);
  if (!checkStatus(fragmentShaderID, glGetShaderiv, glGetShaderInfoLog,
                   glDeleteShader)) {
    return;
  }

  auto programID = glCreateProgram();
  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);
  glLinkProgram(programID);
  if (!checkStatus(programID, glGetProgramiv, glGetProgramInfoLog,
                   glDeleteProgram)) {
    return;
  }
  glUseProgram(programID);
}

MeGLWindow::MeGLWindow() {}

void MeGLWindow::initializeGL() {
  glewExperimental = true;
  glewInit();
  sendDataToOpenGL();
  installShaders();
}

void MeGLWindow::paintGL() {
  glViewport(0, 0, width(), height());
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}
