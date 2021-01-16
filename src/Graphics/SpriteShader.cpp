#include "Graphics/SpriteShader.h"
#include "Graphics/SpriteInfo.h"
#include "Graphics/Shaders/SpriteVertex.h"
#include "Graphics/Shaders/SpriteFragment.h"

const GLint FW::SpriteShader::U_TEXTURE = 0;

FW::SpriteShader::SpriteShader():
  vao{0, 1},
  program{
    __src_Graphics_Shaders_SpriteVertex_glsl,
    __src_Graphics_Shaders_SpriteVertex_glsl_len,
    __src_Graphics_Shaders_SpriteFragment_glsl,
    __src_Graphics_Shaders_SpriteFragment_glsl_len
  }
{
  uModelTransform = glGetUniformLocation(program.get(), "uModelTransform");
  uTexture = glGetUniformLocation(program.get(), "uTexture");
  uSpriteInfoX = glGetUniformLocation(program.get(), "uSpriteInfo.x");
  uSpriteInfoY = glGetUniformLocation(program.get(), "uSpriteInfo.y");
  uSpriteInfoW = glGetUniformLocation(program.get(), "uSpriteInfo.w");
  uSpriteInfoH = glGetUniformLocation(program.get(), "uSpriteInfo.h");
}

void FW::SpriteShader::prepareDraw() const {
  vao.bind();
  program.use();
  glUniform1i(uTexture, U_TEXTURE);
}

void FW::SpriteShader::draw(const SpriteSheet& spriteSheet, const Sprite& sprite, const Transform& transform) const {
  // assumes spriteSheet.prepareDraw has been called before
  const SpriteInfo spriteInfo = spriteSheet.getSpriteInfo(sprite);
  glUniform1f(uSpriteInfoX, spriteInfo.x);
  glUniform1f(uSpriteInfoY, spriteInfo.y);
  glUniform1f(uSpriteInfoW, spriteInfo.w);
  glUniform1f(uSpriteInfoH, spriteInfo.h);
  glUniformMatrix4fv(uModelTransform, 1, GL_FALSE, transform.getMatrix());
  vao.draw();
  // assumes spriteSheet.finishDraw will be called later for proper batching
}

void FW::SpriteShader::finishDraw() const {
  vao.unbind();
  glUseProgram(0);
}
