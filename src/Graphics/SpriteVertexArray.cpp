#include "Graphics/SpriteVertexArray.h"

FW::SpriteVertexArray::SpriteVertexArray(int location) {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  const float vertices[] = {
    -0.5f,  0.5f,
     0.5f,  0.5f,
    -0.5f, -0.5f,
     0.5f, -0.5f
  };
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(location);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

FW::SpriteVertexArray::~SpriteVertexArray() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void FW::SpriteVertexArray::bind() const {
  glBindVertexArray(vao);
}

void FW::SpriteVertexArray::unbind() const {
  glBindVertexArray(0);
}

void FW::SpriteVertexArray::draw() const {
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
