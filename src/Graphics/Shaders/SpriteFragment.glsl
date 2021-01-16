#version 300 es

precision mediump float;

uniform sampler2D uTexture;

struct SpriteShaderInfo {
  float x;
  float y;
  float w;
  float h;
};
uniform SpriteShaderInfo uSpriteShaderInfo;

out vec4 FragColor;

in vec2 texCoord;

void main() {
  vec2 spriteTexCoord = vec2(
    uSpriteShaderInfo.x + texCoord.x * uSpriteShaderInfo.w,
    // the texture iamge is loaded with origin at top right
    (1.0f - uSpriteShaderInfo.y) - texCoord.y * uSpriteShaderInfo.h
  );
  FragColor = texture(uTexture, spriteTexCoord);
}
