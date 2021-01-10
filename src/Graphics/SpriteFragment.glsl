#version 300 es

precision mediump float;

uniform sampler2D uTexture;

out vec4 FragColor;

in vec2 texCoord;

void main() {
  FragColor = texture(uTexture, texCoord);
}
