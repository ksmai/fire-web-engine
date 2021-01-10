#include <iostream>
#include "Graphics/Shader.h"

FW::Shader::Shader(Type type, Source source) {
  shader = glCreateShader(type);
  compile(source);
}

FW::Shader::Shader(Shader&& moved) {
  shader = moved.shader;
  moved.shader = 0;
}

FW::Shader::~Shader() {
  if (shader != 0) {
    glDeleteShader(shader);
  }
}

FW::Shader::Ref FW::Shader::get() const {
  return shader;
}

void FW::Shader::compile(Source source) const {
  const char* glSource{reinterpret_cast<const char*>(source)};
  glShaderSource(shader, 1, &glSource, nullptr);
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

FW::Shader FW::Shader::createVertexShader(Source source) {
  return Shader{GL_VERTEX_SHADER, source};
}

FW::Shader FW::Shader::createFragmentShader(Source source) {
  return Shader{GL_FRAGMENT_SHADER, source};
}
