#include "Graphics/TileShader.h"
#include "Graphics/Shaders/TileVertex.h"
#include "Graphics/Shaders/TileFragment.h"

const GLint FW::TileShader::U_TEXTURE = 0;

FW::TileShader::TileShader():
  program{
    src_Graphics_Shaders_TileVertex_glsl,
    src_Graphics_Shaders_TileVertex_glsl_len,
    src_Graphics_Shaders_TileFragment_glsl,
    src_Graphics_Shaders_TileFragment_glsl_len
  }
{
  uTileInfoGridCellDimension = glGetUniformLocation(program.get(), "uTileInfo.gridCellDimension");
  uTileInfoGridDimension = glGetUniformLocation(program.get(), "uTileInfo.gridDimension");
  uTileInfoSpriteDimension = glGetUniformLocation(program.get(), "uTileInfo.spriteDimension");
  uTileInfoSpriteMargin = glGetUniformLocation(program.get(), "uTileInfo.spriteMargin");
  uTexture = glGetUniformLocation(program.get(), "uTexture");
}

void FW::TileShader::prepareDraw(float cellW, float cellH, float gridW, float gridH, const SpriteSheet& spriteSheet) const {
  program.use();
  glUniform2f(uTileInfoGridCellDimension, cellW, cellH);
  glUniform2f(uTileInfoGridDimension, gridW, gridH);
  glUniform2f(uTileInfoSpriteDimension, spriteSheet.getSpriteWidthInTexCoords(), spriteSheet.getSpriteHeightInTexCoords());
  glUniform2f(uTileInfoSpriteMargin, spriteSheet.getMarginWidthInTexCoords(), spriteSheet.getMarginHeightInTexCoords());
  glUniform1i(uTexture, U_TEXTURE);
  // assumes spriteSheet.prepareDraw and spriteSheet.finishDraw will be called
  // later for proper batching
}

void FW::TileShader::finishDraw() const {
  glUseProgram(0);
}
