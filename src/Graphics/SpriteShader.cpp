#include <iostream>
#include "Graphics/SpriteShader.h"

FW::SpriteShader::SpriteShader() {
  program = 0;
}

FW::SpriteShader::SpriteShader(const Shader& vertexShader, const Shader& fragmentShader) {
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

FW::SpriteShader::SpriteShader(SpriteShader&& other) {
  program = other.program;
  other.program = 0;
}

FW::SpriteShader& FW::SpriteShader::operator=(SpriteShader&& other) {
  std::swap(program, other.program);
  return *this;
}

FW::SpriteShader::~SpriteShader() {
  if (program != 0) {
    glDeleteProgram(program);
  }
}

void FW::SpriteShader::use() const {
  glUseProgram(program);
}

FW::SpriteShader::Ref FW::SpriteShader::get() const {
  return program;
}
