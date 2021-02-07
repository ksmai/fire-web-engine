#version 300 es

layout (location = 0) in vec2 aTileOffset;
layout (location = 1) in vec2 aTexOffset;

struct TileInfo {
  vec2 gridCellDimension;
  vec2 gridDimension;
  vec2 spriteDimension;
  vec2 spriteMargin;
};
uniform TileInfo uTileInfo;

out vec2 texCoord;

void main(void) {
  vec2 gridOffset = vec2(mod(float(gl_InstanceID), uTileInfo.gridDimension.x), floor(float(gl_InstanceID) / uTileInfo.gridDimension.x));
  gl_Position = vec4((gridOffset + aTileOffset) * uTileInfo.gridCellDimension, 0.0f, 1.0f);

  texCoord = (uTileInfo.spriteDimension + uTileInfo.spriteMargin) * aTexOffset
    + uTileInfo.spriteDimension * aTileOffset;
}
