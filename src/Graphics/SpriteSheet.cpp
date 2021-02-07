#include "File/ImageFile.h"
#include "Graphics/SpriteSheet.h"

FW::SpriteSheet::SpriteSheet(
  Data&& data,
  unsigned int spriteWidth,
  unsigned int spriteHeight,
  unsigned int margin
):
  texture{ImageFile{std::move(data)}},
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

FW::Sprite FW::SpriteSheet::makeSprite(const SpritePosition& spritePosition) const {
  return Sprite{getSpriteShaderInfo(spritePosition)};
}

const FW::SpriteShaderInfo FW::SpriteSheet::getSpriteShaderInfo(const SpritePosition& spritePosition) const {
  // x, y represents the offest in terms of number of sprites from the top-left
  // corner of the spritesheet
  // we need to translate them to [0, 1] texture coordinates with origin at
  // bottom right
  SpriteShaderInfo result;
  float textureWidth = static_cast<float>(texture.getWidth());
  float textureHeight = static_cast<float>(texture.getHeight());
  float totalHeight = static_cast<float>((spriteHeight + margin) * (spritePosition.spanY - 1) + spriteHeight);
  result.x = static_cast<float>(spritePosition.x * (spriteWidth + margin)) / textureWidth;
  result.y = 1.0f - static_cast<float>(spritePosition.y * (spriteHeight + margin) + totalHeight) / textureHeight;
  result.w = static_cast<float>((spriteWidth + margin) * (spritePosition.spanX - 1) + spriteWidth) / textureWidth;
  result.h = totalHeight / textureHeight;
  return result;
}

float FW::SpriteSheet::getSpriteWidthInTexCoords() const {
  return static_cast<float>(spriteWidth) / texture.getWidth();
}

float FW::SpriteSheet::getSpriteHeightInTexCoords() const {
  return static_cast<float>(spriteHeight) / texture.getHeight();
}

float FW::SpriteSheet::getMarginWidthInTexCoords() const {
  return static_cast<float>(margin) / texture.getWidth();
}

float FW::SpriteSheet::getMarginHeightInTexCoords() const {
  return static_cast<float>(margin) / texture.getHeight();
}
