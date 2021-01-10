#include "Graphics/SpriteSheet.h"

FW::SpriteSheet::SpriteSheet(
  Texture&& texture,
  unsigned int spriteWidth,
  unsigned int spriteHeight,
  unsigned int margin
):
  texture{std::move(texture)},
  spriteWidth{spriteWidth},
  spriteHeight{spriteHeight},
  margin{margin}
{
}

void FW::SpriteSheet::prepareDraw(int unit) const {
  texture.bind(unit);
}

void FW::SpriteSheet::finishDraw(int unit) const {
  texture.unbind(unit);
}

const FW::SpriteInfo FW::SpriteSheet::getSpriteInfo(const Sprite& sprite) const {
  // x, y represents the offest in terms of number of sprites from the top-left
  // corner of the spritesheet
  // we need to translate them to [0, 1] texture coordinates with origin at
  // bottom right
  SpriteInfo result;
  float textureWidth = static_cast<float>(texture.getWidth());
  float textureHeight = static_cast<float>(texture.getHeight());
  float totalHeight = static_cast<float>((spriteHeight + margin) * (sprite.spanY - 1) + spriteHeight);
  result.x = static_cast<float>(sprite.x * (spriteWidth + margin)) / textureWidth;
  result.y = 1.0f - static_cast<float>(sprite.y * (spriteHeight + margin) + totalHeight) / textureHeight;
  result.w = static_cast<float>((spriteWidth + margin) * (sprite.spanX - 1) + spriteWidth) / textureWidth;
  result.h = totalHeight / textureHeight;
  return result;
}
