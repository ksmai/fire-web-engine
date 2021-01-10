#ifndef __SPRITE_SHADER_H__
#define __SPRITE_SHADER_H__

#include "GLES3/gl3.h"
#include "Graphics/Shader.h"

namespace FW {
  class SpriteShader {
  public:
    using Ref = GLuint;

    SpriteShader();

    SpriteShader(const Shader& vertexShader, const Shader& fragmentShader);

    SpriteShader(SpriteShader&&);

    SpriteShader& operator=(SpriteShader&&);

    SpriteShader(const SpriteShader&) =delete;

    SpriteShader& operator=(const SpriteShader&) =delete;

    ~SpriteShader();

    void use() const;

    Ref get() const;

  private:
    GLuint program;
  };
}

#endif
