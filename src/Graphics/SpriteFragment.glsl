#version 300 es

precision mediump float;

uniform sampler2D uTexture;

struct SpriteInfo {
  float x;
  float y;
  float w;
  float h;
};
uniform SpriteInfo uSpriteInfo;

out vec4 FragColor;

in vec2 texCoord;

void main() {
  vec2 spriteTexCoord = vec2(
    uSpriteInfo.x + texCoord.x * uSpriteInfo.w,
    // the texture iamge is loaded with origin at top right
    (1.0f - uSpriteInfo.y) - texCoord.y * uSpriteInfo.h
  );
  FragColor = texture(uTexture, spriteTexCoord);
}
