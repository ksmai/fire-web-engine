#ifndef __SPRITE_VERTEX_ARRAY__
#define __SPRITE_VERTEX_ARRAY__

#include "GLES3/gl3.h"

namespace FW {
  class SpriteVertexArray {
  public:
    SpriteVertexArray(int location);
    SpriteVertexArray(const SpriteVertexArray&) =delete;
    SpriteVertexArray& operator=(const SpriteVertexArray&) =delete;
    ~SpriteVertexArray();

    void bind() const;

    void unbind() const;

    void draw() const;

  private:
    GLuint vao;
    GLuint vbo;
  };
}
#endif
