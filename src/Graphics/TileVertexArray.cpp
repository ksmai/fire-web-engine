#include "Graphics/TileVertexArray.h"

FW::TileVertexArray::TileVertexArray(const std::vector<float>& texOffsets):
  numInstances{texOffsets.size() / 2}
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &tileOffsetVBO);
  glGenBuffers(1, &texOffsetVBO);

  const float tileOffsets[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
  };

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, tileOffsetVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tileOffsets), tileOffsets, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, texOffsetVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texOffsets.size(), &texOffsets[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glVertexAttribDivisor(1, 1);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

FW::TileVertexArray::~TileVertexArray() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glDeleteBuffers(1, &texOffsetVBO);
  glDeleteBuffers(1, &tileOffsetVBO);
  glDeleteVertexArrays(1, &vao);
}

void FW::TileVertexArray::draw() const {
  glBindVertexArray(vao);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numInstances);
  glBindVertexArray(0);
}
