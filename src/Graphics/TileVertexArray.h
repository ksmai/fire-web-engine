#ifndef __TILE_VERTEX_ARRAY__
#define __TILE_VERTEX_ARRAY__

#include <vector>
#include "GLES3/gl3.h"

namespace FW {
  class TileVertexArray {
  public:
    TileVertexArray(const std::vector<float>& texOffsets);
    TileVertexArray(const TileVertexArray&) =delete;
    TileVertexArray& operator=(const TileVertexArray&) =delete;
    ~TileVertexArray();

    void draw() const;

  private:
    GLuint vao;
    GLuint tileOffsetVBO;
    GLuint texOffsetVBO;

    unsigned numInstances;
  };
}
#endif
