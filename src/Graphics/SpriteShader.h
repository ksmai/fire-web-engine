#ifndef __SPRITE_SHADER_H__
#define __SPRITE_SHADER_H__

#include "glm/glm.hpp"
#include "GLES3/gl3.h"
#include "Graphics/SpriteVertexArray.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

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

    void draw(const Texture&, const glm::mat4&) const;

    void finishDraw() const;

  private:
    SpriteVertexArray vao{0};
    GLuint program;
    GLuint uModelTransform;
    GLuint uTexture;

    static const GLint U_TEXTURE;
  };
}

#endif
