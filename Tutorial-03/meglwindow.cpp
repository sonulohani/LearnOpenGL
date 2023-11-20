#include <GL/glew.h>
#include <fstream>
#include <functional>
#include <iostream>

#include "meglwindow.h"

const float X_DELTA = 0.1f;
const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_VERTICES = 6;
const uint TRIANGLE_BYTE_SIZE =
    NUM_VERTICES_PER_TRI * NUM_FLOATS_VERTICES * sizeof(float);
uint numTris = 0;
const uint MAX_TRIS = 20;

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
  GLuint vertexBufferID;
  glGenBuffers(1, &vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, MAX_TRIS * TRIANGLE_BYTE_SIZE, nullptr,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (void *)(sizeof(float) * 3));
}

void installShaders() {
  GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  const char *adapter[1];
  std::string temp = readShaderCode("shaders/VertexShaderCode.vert");
  adapter[0] = temp.c_str();
  glShaderSource(vertexShaderID, 1, adapter, 0);
  temp = readShaderCode("shaders/FragmentShaderCode.frag");
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

void sendAnotherTriToOpenGL() {
  if (numTris == MAX_TRIS) {
    return;
  }

  const GLfloat THIS_TRI_X = -1 + numTris * X_DELTA;
  GLfloat thisTri[] = {
      THIS_TRI_X,
      1.f,
      0.f,

      1.f,
      0.f,
      0.f,

      THIS_TRI_X + X_DELTA,
      1.f,
      0.f,

      1.f,
      0.f,
      0.f,

      THIS_TRI_X,
      0.f,
      0.f,

      1.f,
      0.f,
      0.f,
  };

  glBufferSubData(GL_ARRAY_BUFFER, numTris * TRIANGLE_BYTE_SIZE,
                  TRIANGLE_BYTE_SIZE, thisTri);
  numTris++;
}

void MeGLWindow::initializeGL() {
  glewExperimental = true;
  glewInit();
  glEnable(GL_DEPTH_TEST);
  sendDataToOpenGL();
  installShaders();
}

void MeGLWindow::paintGL() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  //  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width(), height());
  sendAnotherTriToOpenGL();
  //  glDrawArrays(GL_TRIANGLES, (numTris - 1) * NUM_VERTICES_PER_TRI,
  //               NUM_VERTICES_PER_TRI);
  //  glDrawArrays(GL_TRIANGLES, 0, numTris * NUM_VERTICES_PER_TRI);
  glDrawArrays(GL_TRIANGLES, (numTris - 1) * NUM_VERTICES_PER_TRI,
               NUM_VERTICES_PER_TRI);
}
