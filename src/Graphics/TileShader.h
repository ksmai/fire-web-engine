#ifndef __TILE_SHADER_H__
#define __TILE_SHADER_H__

#include "GLES3/gl3.h"
#include "Graphics/Program.h"
#include "Graphics/SpriteSheet.h"

namespace FW {
  class TileShader {
  public:
    TileShader();
    TileShader(const TileShader&) =delete;
    TileShader& operator=(const TileShader&) =delete;
    ~TileShader() =default;

    void prepareDraw(float cellW, float cellH, float gridW, float gridH, const SpriteSheet& spriteSheet) const;
    void finishDraw() const;

  private:
    Program program;
    GLuint uTileInfoGridCellDimension;
    GLuint uTileInfoGridDimension;
    GLuint uTileInfoSpriteDimension;
    GLuint uTileInfoSpriteMargin;
    GLuint uTexture;

    static const GLint U_TEXTURE;
  };
}

#endif
