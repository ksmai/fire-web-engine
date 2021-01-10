#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uModelTransform;

out vec2 texCoord;

void main() {
  gl_Position = uModelTransform * vec4(aPos, 1.0);
  texCoord = vec2(aTexCoord.x, 1.0-aTexCoord.y);
}
