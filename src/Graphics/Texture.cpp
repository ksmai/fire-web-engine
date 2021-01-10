#include "Graphics/Texture.h"

FW::Texture::Texture(const Resource& resource):
  Texture{dynamic_cast<const ImageResource&>(resource)}
{
}

FW::Texture::Texture(const ImageResource& img):
  width{img.width()},
  height{img.height()}
{
  glGenTextures(1, &texture);
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  GLint format = GL_RGB;
  if (img.colors() == 4) {
    format = GL_RGBA;
  } else if (img.colors() == 3) {
    format = GL_RGB;
  } else {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "unknown pixel format");
    throw "unknown pixel format";
  }
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img.buffer());
  unbind();
}

FW::Texture::Texture(Texture&& other):
  texture{other.texture},
  width{other.width},
  height{other.height}
{
  other.texture = 0;
  other.width = 0;
  other.height = 0;
}

FW::Texture::~Texture() {
  if (texture != 0) {
    glDeleteTextures(1, &texture);
  }
}

void FW::Texture::bind(int unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, texture);
}

void FW::Texture::unbind(int unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int FW::Texture::getWidth() const {
  return width;
}

unsigned int FW::Texture::getHeight() const {
  return height;
}
