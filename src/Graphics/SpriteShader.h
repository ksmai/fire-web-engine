#ifndef __SPRITE_SHADER_H__
#define __SPRITE_SHADER_H__

#include "GLES3/gl3.h"
#include "Graphics/SpriteVertexArray.h"
#include "Graphics/Program.h"
#include "Graphics/SpriteSheet.h"
#include "Graphics/SpritePosition.h"
#include "Graphics/Transform.h"

namespace FW {
  class SpriteShader {
  public:
    SpriteShader();
    SpriteShader(const SpriteShader&) =delete;
    SpriteShader& operator=(const SpriteShader&) =delete;
    ~SpriteShader() =default;

    void prepareDraw() const;
    void draw(const SpriteSheet&, const SpritePosition&, const Transform&) const;
    void finishDraw() const;

  private:
    SpriteVertexArray vao;
    Program program;
    GLuint uModelTransform;
    GLuint uTexture;
    GLuint uSpriteShaderInfoX;
    GLuint uSpriteShaderInfoY;
    GLuint uSpriteShaderInfoW;
    GLuint uSpriteShaderInfoH;

    static const GLint U_TEXTURE;
  };
}

#endif
