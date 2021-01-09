#include <SDL/SDL.h>
#include "SDL_image.h"
#include "Resource/ImageLoader.h"
#include "Resource/ImageResource.h"

std::regex FW::ImageLoader::pattern() const {
  return std::regex{".*\\.png"};
}

FW::Resource* FW::ImageLoader::load(Resource::Data&& data) const {
  SDL_RWops* rw{SDL_RWFromConstMem(&data[0], data.size())};
  if (!rw) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_RWFromConstMem failed: %s",  SDL_GetError());
    throw "SDL_RWFromConstMem failed";
  }
  SDL_Surface* img{IMG_Load_RW(rw, 1)};
  if (!img) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Load_RW failed: %s", IMG_GetError());
    throw "IMG_Load_RW failed";
  }
  SDL_Log("image loaded: width: %d, height: %d", img->w, img->h);
  SDL_Log("bytes per pixel = %d, Rmask = %x, Bmask = %x, Gmask = %x, Amask = %x",
    img->format->BytesPerPixel,
    img->format->Rmask, img->format->Gmask, img->format->Bmask, img->format->Amask);
  return new ImageResource{img};
}
