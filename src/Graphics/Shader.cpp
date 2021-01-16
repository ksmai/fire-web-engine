#include <string>
#include "App/Logger.h"
#include "App/abort.h"
#include "Graphics/Shader.h"

FW::Shader::Shader(GLuint type, Source source, Length length) {
  shader = glCreateShader(type);
  compile(source, length);
}

FW::Shader::~Shader() {
  if (shader != 0) {
    glDeleteShader(shader);
  }
}

GLuint FW::Shader::get() const {
  return shader;
}

void FW::Shader::compile(Source source, Length length) const {
  std::string glSource{source, source + length};
  const char* ptr{glSource.c_str()};
  glShaderSource(shader, 1, &ptr, nullptr);
  glCompileShader(shader);
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    Logger::error("GLSL Shader compilation failed: %s", infoLog);
    abort();
  }
}

FW::Shader FW::Shader::createVertexShader(Source source, Length length) {
  return Shader{GL_VERTEX_SHADER, source, length};
}

FW::Shader FW::Shader::createFragmentShader(Source source, Length length) {
  return Shader{GL_FRAGMENT_SHADER, source, length};
}
