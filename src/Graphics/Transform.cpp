#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Graphics/Transform.h"

FW::Transform::Transform():
  matrix{1.0f},
  position{0.0f, 0.0f},
  size{1.0f, 1.0f},
  radians{0.0f},
  dirty{false}
{
}

void FW::Transform::translate(float x, float y) {
  position.x += x;
  position.y += y;
  dirty = true;
}

void FW::Transform::scale(float s) {
  scale(s, s);
}

void FW::Transform::scale(float x, float y) {
  size.x *= x;
  size.y *= y;
  dirty = true;
}

void FW::Transform::rotate(float r) {
  radians += r;
  dirty = true;
}

const float* FW::Transform::getMatrix() const {
  if (dirty) {
    matrix = glm::scale(
      glm::rotate(
        glm::translate(
          glm::mat4{1.0f},
          glm::vec3{position.x, position.y, 0.0f}
        ),
        radians,
        glm::vec3{0.0f, 0.0f, 1.0f}
      ),
      glm::vec3{size.x, size.y, 1.0f}
    );
    dirty = false;
  }
  return glm::value_ptr(matrix);
}
