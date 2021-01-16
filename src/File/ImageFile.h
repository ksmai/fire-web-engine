#ifndef __IMAGE_FILE_H__
#define __IMAGE_FILE_H__

#include "SDL/SDL.h"
#include "File/Data.h"

namespace FW {
  class ImageFile {
  public:
    using Size = std::size_t;

    ImageFile(Data&& data);
    ImageFile(const ImageFile&) =delete;
    ImageFile& operator=(const ImageFile&) =delete;
    ~ImageFile();

    Buffer getTempBuffer() const;
    Size getSize() const;
    Size getWidth() const;
    Size getHeight() const;
    bool hasAlpha() const;

  private:
    void loadImage(Data&& data);

    SDL_Surface* surface;
  };
}

#endif
