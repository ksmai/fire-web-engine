#include <iostream>
#include "Shader/Shader.h"

Shader::Shader(Type type, Source source) {
  shader = glCreateShader(type);
  compile(source);
}

Shader::Shader(Shader&& moved) {
  shader = moved.shader;
  moved.shader = 0;
}

Shader::~Shader() {
  if (shader != 0) {
    glDeleteShader(shader);
  }
}

Shader::Ref Shader::get() const {
  return shader;
}

void Shader::compile(Source source) const {
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cout << "GLSL Shader compilation failed: " << infoLog << "\n";
    throw "Shader compilation failed";
  }
}

Shader Shader::createVertexShader(Source source) {
  return Shader{GL_VERTEX_SHADER, source};
}

Shader Shader::createFragmentShader(Source source) {
  return Shader{GL_FRAGMENT_SHADER, source};
}
