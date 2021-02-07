#ifndef __SPRITE_SHEET_H__
#define __SPRITE_SHEET_H__

#include "File/Data.h"
#include "Graphics/SpritePosition.h"
#include "Graphics/SpriteShaderInfo.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"

namespace FW {
  class SpriteSheet {
  public:
    SpriteSheet(Data&& data, unsigned int spriteWidth, unsigned int spriteHeight, unsigned int margin = 0);

    void prepareDraw(int unit = 0) const;
    void finishDraw(int unit = 0) const;

    Sprite makeSprite(const SpritePosition&) const;

    float getSpriteWidthInTexCoords() const;
    float getSpriteHeightInTexCoords() const;
    float getMarginWidthInTexCoords() const;
    float getMarginHeightInTexCoords() const;

  private:
    const SpriteShaderInfo getSpriteShaderInfo(const SpritePosition&) const;

    Texture texture;
    const unsigned int spriteWidth;
    const unsigned int spriteHeight;
    const unsigned int margin;
  };
}
#endif
