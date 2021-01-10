#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Graphics/SpriteShader.h"

const GLint FW::SpriteShader::U_TEXTURE = 0;

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

  uModelTransform = glGetUniformLocation(program, "uModelTransform");
  uTexture = glGetUniformLocation(program, "uTexture");
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
  glDeleteProgram(program);
}

void FW::SpriteShader::prepareDraw() const {
  vao.bind();
  glUseProgram(program);
  glUniform1i(uTexture, U_TEXTURE);
}

void FW::SpriteShader::draw(const Texture& texture, const glm::mat4& transform) const {
  texture.bind(U_TEXTURE);
  glUniformMatrix4fv(uModelTransform, 1, GL_FALSE, glm::value_ptr(transform));
  glUniform1f(glGetUniformLocation(program, "uSpriteInfo.x"), 0.0f);
  glUniform1f(glGetUniformLocation(program, "uSpriteInfo.y"), 0.0f);
  glUniform1f(glGetUniformLocation(program, "uSpriteInfo.w"), 1.0f);
  glUniform1f(glGetUniformLocation(program, "uSpriteInfo.h"), 1.0f);
  vao.draw();
  texture.unbind(U_TEXTURE);
}

void FW::SpriteShader::finishDraw() const {
  vao.unbind();
  glUseProgram(0);
}
