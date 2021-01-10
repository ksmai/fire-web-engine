#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "GLES3/gl3.h"
#include "Resource/ImageResource.h"

namespace FW {
  class Texture {
  public:
    Texture(const Resource&);
    Texture(const ImageResource&);
    ~Texture();

    void bind(int unit = 0);
    void unbind() const;

  private:
    GLuint texture;
    int unit{0};
  };
}
#endif
