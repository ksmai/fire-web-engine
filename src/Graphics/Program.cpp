#include "App/Logger.h"
#include "App/abort.h"
#include "Graphics/Program.h"

FW::Program::Program(
  Shader::Source vertexShaderSource,
  Shader::Length vertexShaderLength,
  Shader::Source fragmentShaderSource,
  Shader::Length fragmentShaderLength
) {
  program = glCreateProgram();
  Shader vertexShader{Shader::createVertexShader(vertexShaderSource, vertexShaderLength)};
  Shader fragmentShader{Shader::createFragmentShader(fragmentShaderSource, fragmentShaderLength)};
  glAttachShader(program, vertexShader.get());
  glAttachShader(program, fragmentShader.get());
  glLinkProgram(program);
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    Logger::error("GLSL program link failed: %s", infoLog);
    abort();
  }
}

FW::Program::~Program() {
  if (program != 0) {
    glDeleteProgram(program);
  }
}

GLuint FW::Program::get() const {
  return program;
}

void FW::Program::use() const {
  glUseProgram(program);
}
