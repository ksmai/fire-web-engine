#version 300 es

precision mediump float;

uniform vec3 uColor;
uniform sampler2D uTexture;

out vec4 FragColor;

in vec2 texCoord;

void main() {
  FragColor = texture(uTexture, texCoord) * vec4(uColor, 1.0);
}
