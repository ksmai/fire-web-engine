#include <iostream>
#include "Shader/Program.h"

Program::Program() {
  program = 0;
}

Program::Program(const Shader& vertexShader, const Shader& fragmentShader) {
  program = glCreateProgram();
  glAttachShader(program, vertexShader.get());
  glAttachShader(program, fragmentShader.get());
  glLinkProgram(program);
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cout << "GLSL program link failed: " << infoLog << "\n";
    throw "GLSL program link failed";
  }
}

Program::Program(Program&& other) {
  program = other.program;
  other.program = 0;
}

Program& Program::operator=(Program&& other) {
  std::swap(program, other.program);
  return *this;
}

Program::~Program() {
  if (program != 0) {
    glDeleteProgram(program);
  }
}

void Program::use() const {
  glUseProgram(program);
}

Program::Ref Program::get() const {
  return program;
}
