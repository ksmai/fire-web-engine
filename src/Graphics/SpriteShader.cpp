#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Graphics/SpriteShader.h"
#include "Graphics/SpriteInfo.h"

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
  uSpriteInfoX = glGetUniformLocation(program, "uSpriteInfo.x");
  uSpriteInfoY = glGetUniformLocation(program, "uSpriteInfo.y");
  uSpriteInfoW = glGetUniformLocation(program, "uSpriteInfo.w");
  uSpriteInfoH = glGetUniformLocation(program, "uSpriteInfo.h");
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

void FW::SpriteShader::draw(const SpriteSheet& spriteSheet, const Sprite& sprite, const glm::mat4& transform) const {
  // assumes spriteSheet.prepareDraw has been called before
  const SpriteInfo spriteInfo = spriteSheet.getSpriteInfo(sprite);
  glUniform1f(uSpriteInfoX, spriteInfo.x);
  glUniform1f(uSpriteInfoY, spriteInfo.y);
  glUniform1f(uSpriteInfoW, spriteInfo.w);
  glUniform1f(uSpriteInfoH, spriteInfo.h);
  glUniformMatrix4fv(uModelTransform, 1, GL_FALSE, glm::value_ptr(transform));
  vao.draw();
  // assumes spriteSheet.prepareDraw will be called later for proper batching
}

void FW::SpriteShader::finishDraw() const {
  vao.unbind();
  glUseProgram(0);
}
