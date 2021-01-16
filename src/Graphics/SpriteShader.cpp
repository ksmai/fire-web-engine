#include "Graphics/SpriteShader.h"
#include "Graphics/SpriteShaderInfo.h"
#include "Graphics/Shaders/SpriteVertex.h"
#include "Graphics/Shaders/SpriteFragment.h"

const GLint FW::SpriteShader::U_TEXTURE = 0;

FW::SpriteShader::SpriteShader():
  vao{0, 1},
  program{
    ___src_Graphics_Shaders_SpriteVertex_glsl,
    ___src_Graphics_Shaders_SpriteVertex_glsl_len,
    ___src_Graphics_Shaders_SpriteFragment_glsl,
    ___src_Graphics_Shaders_SpriteFragment_glsl_len
  }
{
  uModelTransform = glGetUniformLocation(program.get(), "uModelTransform");
  uTexture = glGetUniformLocation(program.get(), "uTexture");
  uSpriteShaderInfoX = glGetUniformLocation(program.get(), "uSpriteShaderInfo.x");
  uSpriteShaderInfoY = glGetUniformLocation(program.get(), "uSpriteShaderInfo.y");
  uSpriteShaderInfoW = glGetUniformLocation(program.get(), "uSpriteShaderInfo.w");
  uSpriteShaderInfoH = glGetUniformLocation(program.get(), "uSpriteShaderInfo.h");
}

void FW::SpriteShader::prepareDraw() const {
  vao.bind();
  program.use();
  glUniform1i(uTexture, U_TEXTURE);
}

void FW::SpriteShader::draw(const SpriteSheet& spriteSheet, const SpritePosition& spritePosition, const Transform& transform) const {
  // assumes spriteSheet.prepareDraw has been called before
  const SpriteShaderInfo spriteShaderInfo = spriteSheet.getSpriteShaderInfo(spritePosition);
  glUniform1f(uSpriteShaderInfoX, spriteShaderInfo.x);
  glUniform1f(uSpriteShaderInfoY, spriteShaderInfo.y);
  glUniform1f(uSpriteShaderInfoW, spriteShaderInfo.w);
  glUniform1f(uSpriteShaderInfoH, spriteShaderInfo.h);
  glUniformMatrix4fv(uModelTransform, 1, GL_FALSE, transform.getMatrix());
  vao.draw();
  // assumes spriteSheet.finishDraw will be called later for proper batching
}

void FW::SpriteShader::finishDraw() const {
  vao.unbind();
  glUseProgram(0);
}
