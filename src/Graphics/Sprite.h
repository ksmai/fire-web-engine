#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Graphics/SpriteShaderInfo.h"
#include "Graphics/Transform.h"

namespace FW {
  class Sprite {
  public:
    friend class SpriteShader;

    explicit Sprite(const SpriteShaderInfo&);
    Sprite(const Sprite&) =delete;
    Sprite& operator=(const Sprite&) =delete;
    Sprite(Sprite&&) =default;

  private:
    SpriteShaderInfo spriteShaderInfo;
    Transform transform;
  };
}
#endif
