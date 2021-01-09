#ifndef __IMAGE_RESOURCE_H__
#define __IMAGE_RESOURCE_H__

#include <SDL/SDL.h>
#include <SDL_image.h>
#include "Resource/Resource.h"

namespace FW {
  class ImageResource: public Resource {
  public:
    ImageResource(SDL_Surface* img): img{img} {
    }

    virtual ~ImageResource() {
      SDL_FreeSurface(img);
    }

    virtual Resource::Buffer buffer() const override {
      return static_cast<Resource::Buffer>(img->pixels);
    }

    virtual Resource::Size size() const override {
      return img->pitch * img->h;
    }

  private:
    SDL_Surface* img;
  };
}
#endif
