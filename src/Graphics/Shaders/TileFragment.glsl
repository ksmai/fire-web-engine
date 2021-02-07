#version 300 es

precision mediump float;

in vec2 texCoord;

uniform sampler2D uTexture;

out vec4 FragColor;

void main(void) {
  FragColor = texture(uTexture, vec2(texCoord.x, 1.0f - texCoord.y));
}
