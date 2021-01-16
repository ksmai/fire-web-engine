#include "SDL_image.h"
#include "File/ImageFile.h"
#include "App/Logger.h"
#include "App/abort.h"

FW::ImageFile::ImageFile(Data&& data) {
  loadImage(std::move(data));
}

FW::ImageFile::~ImageFile() {
  if (surface) {
    SDL_FreeSurface(surface);
  }
}

FW::Buffer FW::ImageFile::getTempBuffer() const {
  return static_cast<FW::Buffer>(surface->pixels);
}

FW::ImageFile::Size FW::ImageFile::getSize() const {
  return surface->pitch * surface ->h;
}

FW::ImageFile::Size FW::ImageFile::getWidth() const {
  return surface->w;
}

FW::ImageFile::Size FW::ImageFile::getHeight() const {
  return surface->h;
}

bool FW::ImageFile::hasAlpha() const {
  return surface->format->BytesPerPixel == 4;
}

void FW::ImageFile::loadImage(Data&& data) {
  SDL_RWops* rw{SDL_RWFromConstMem(&data[0], data.size())};
  if (!rw) {
    Logger::error("SDL_RWFromConstMem failed: %s",  SDL_GetError());
    abort();
  }
  surface = IMG_Load_RW(rw, 1);
  if (!surface) {
    Logger::error("IMG_Load_RW failed: %s", IMG_GetError());
    abort();
  }
  Logger::info("image loaded: width: %d, height: %d", surface->w, surface->h);
  Logger::info("bytes per pixel = %d, Rmask = %x, Bmask = %x, Gmask = %x, Amask = %x",
    surface->format->BytesPerPixel,
    surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
}

