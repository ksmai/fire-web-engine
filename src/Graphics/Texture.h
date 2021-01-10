#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "GLES3/gl3.h"
#include "Resource/ImageResource.h"

namespace FW {
  class Texture {
  public:
    Texture(const Resource&);
    Texture(const ImageResource&);
    Texture(const Texture&) =delete;
    Texture(Texture&&);
    Texture& operator=(const Texture&) = delete;
    ~Texture();

    void bind(int unit = 0) const;
    void unbind(int unit = 0) const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;

  private:
    GLuint texture;
    unsigned int width;
    unsigned int height;
  };
}
#endif
