#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "glm/glm.hpp"

namespace FW {
  class Transform {
  public:
    Transform();

    void translate(float, float);
    void scale(float, float);
    void scale(float);
    void rotate(float);
    const float* getMatrix() const;

  private:
    mutable glm::mat4 matrix;
    glm::vec2 position;
    glm::vec2 size;
    float radians;
    mutable bool dirty;
  };
}
#endif
