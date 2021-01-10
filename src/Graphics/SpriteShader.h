#ifndef __SPRITE_SHADER_H__
#define __SPRITE_SHADER_H__

#include "GLES3/gl3.h"
#include "Graphics/SpriteVertexArray.h"
#include "Graphics/Shader.h"
#include "Graphics/SpriteSheet.h"
#include "Graphics/Sprite.h"
#include "Graphics/Transform.h"

namespace FW {
  class SpriteShader {
  public:
    SpriteShader(const Shader& vertexShader, const Shader& fragmentShader);

    SpriteShader(SpriteShader&&);

    SpriteShader& operator=(SpriteShader&&);

    SpriteShader(const SpriteShader&) =delete;

    SpriteShader& operator=(const SpriteShader&) =delete;

    ~SpriteShader();

    void prepareDraw() const;

    void draw(const SpriteSheet&, const Sprite&, const Transform&) const;

    void finishDraw() const;

  private:
    SpriteVertexArray vao{0, 1};
    GLuint program;
    GLuint uModelTransform;
    GLuint uTexture;
    GLuint uSpriteInfoX;
    GLuint uSpriteInfoY;
    GLuint uSpriteInfoW;
    GLuint uSpriteInfoH;

    static const GLint U_TEXTURE;
  };
}

#endif
