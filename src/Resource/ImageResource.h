#ifndef __IMAGE_RESOURCE_H__
#define __IMAGE_RESOURCE_H__

#include <SDL/SDL.h>
#include <SDL_image.h>
#include "Resource/Resource.h"

namespace FW {
  class ImageResource: public Resource {
  public:
    ImageResource(SDL_Surface* img);

    virtual ~ImageResource();

    virtual Resource::Buffer buffer() const override;

    virtual Resource::Size size() const override;

    std::size_t width() const;
    std::size_t height() const;
    std::size_t colors() const;
    unsigned int redMask() const;

  private:
    SDL_Surface* img;
  };
}
#endif
