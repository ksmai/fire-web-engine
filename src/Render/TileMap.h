#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include <vector>
#include "File/XMLFile.h"
#include "Graphics/Graphics.h"
#include "Graphics/TileVertexArray.h"

namespace FW {
  class TileMap {
  public:
    static TileMap parse(XMLFile&& xmlFile);

    TileMap(const std::vector<float>& texOffsets, std::size_t spriteSheetID, float cellW, float cellH, float gridW, float gridH);
    TileMap(const TileMap&) =delete;
    TileMap& operator=(const TileMap&) =delete;
    TileMap(TileMap&&);

    void draw() const;
    std::size_t getSpriteSheetID() const;
    float getCellW() const;
    float getCellH() const;
    float getGridW() const;
    float getGridH() const;

  private:
    TileVertexArray vao;
    std::size_t spriteSheetID;
    float cellW;
    float cellH;
    float gridW;
    float gridH;
  };
}
#endif
