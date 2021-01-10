#include "Resource/ImageResource.h"

FW::ImageResource::ImageResource(SDL_Surface* img):
  img{img}
{
}

FW::ImageResource::~ImageResource() {
  SDL_FreeSurface(img);
}

FW::Resource::Buffer FW::ImageResource::buffer() const {
  return static_cast<Resource::Buffer>(img->pixels);
}

FW::Resource::Size FW::ImageResource::size() const {
  return img->pitch * img->h;
}

std::size_t FW::ImageResource::width() const {
  return img->w;
}

std::size_t FW::ImageResource::height() const {
  return img->h;
}

std::size_t FW::ImageResource::colors() const {
  return img->format->BytesPerPixel;
}

unsigned int FW::ImageResource::redMask() const {
  return img->format->Rmask;
}
