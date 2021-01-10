#ifndef __SPRITE_SHEET_H__
#define __SPRITE_SHEET_H__

#include "memory"
#include "Graphics/Texture.h"
#include "Graphics/Sprite.h"
#include "Graphics/SpriteInfo.h"

namespace FW {
  class SpriteSheet {
  public:
    SpriteSheet(Texture&& texture, unsigned int spriteWidth, unsigned int spriteHeight, unsigned int margin = 0);

    void prepareDraw(int unit = 0) const;
    const SpriteInfo getSpriteInfo(const Sprite&) const;
    void finishDraw(int unit = 0) const;

  private:
    Texture texture;
    const unsigned int spriteWidth;
    const unsigned int spriteHeight;
    const unsigned int margin;
  };
}
#endif
