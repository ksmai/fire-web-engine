#include "Graphics/SpriteVertexArray.h"

FW::SpriteVertexArray::SpriteVertexArray(int vertexLocation, int texCoordLocation) {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  const float vertices[] = {
    -0.5f,  0.5f,  0.0f,  1.0f,
     0.5f,  0.5f,  1.0f,  1.0f,
    -0.5f, -0.5f,  0.0f,  0.0f,
     0.5f, -0.5f,  1.0f,  0.0f
  };
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(vertexLocation);
  glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(texCoordLocation);
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
