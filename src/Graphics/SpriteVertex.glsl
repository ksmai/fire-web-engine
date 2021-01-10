#version 300 es

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uModelTransform;

out vec2 texCoord;

void main() {
  gl_Position = uModelTransform * vec4(aPos.x, aPos.y, 0.0f, 1.0f);
  texCoord = aTexCoord;
}
