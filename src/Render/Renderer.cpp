#include <functional>
#include "Render/Renderer.h"

#include "Graphics/TileVertexArray.h"

FW::Renderer::Renderer(EventBus* eventBus, Graphics* graphics):
  eventBus{eventBus},
  graphics{graphics},
  spriteSheetRepository{graphics}
{
  addTilePositionListenerID = eventBus->subscribe<AddTilePosition>(
    std::bind(&Renderer::handleAddTilePosition, this, std::placeholders::_1)
  );
}

FW::Renderer::~Renderer() {
  eventBus->unsubscribe<AddTilePosition>(addTilePositionListenerID);
}

void FW::Renderer::init(ZipFile& resources) {
  spriteSheetRepository.load(resources);
}

void FW::Renderer::setTileSheet(SpriteSheetRepository::ID id) {
  tileSpriteSheet = &spriteSheetRepository.get(id);
  tiles.clear();
}

void FW::Renderer::update(double, double) {
  // hardcoded tests
  graphics->prepareDraw();
  graphics->prepareDrawTile(0.1f, 0.1f, 3.0f, 4.0f, *tileSpriteSheet);
  tileSpriteSheet->prepareDraw();
  std::vector<float> texOffsets{
    7.0f, 7.0f,
    8.0f, 7.0f,
    9.0f, 7.0f,
    7.0f, 8.0f,
    8.0f, 8.0f,
    9.0f, 8.0f,
    7.0f, 9.0f,
    8.0f, 9.0f,
    9.0f, 9.0f,
    13.0f, 19.0f,
    14.0f, 19.0f,
    15.0f, 19.0f
  };
  TileVertexArray vao{texOffsets};
  vao.draw();
  tileSpriteSheet->finishDraw();
  graphics->finishDrawTile();
  graphics->finishDraw();
}

void FW::Renderer::handleAddTilePosition(const AddTilePosition& payload) {
  if (!tileSpriteSheet) {
    return;
  }
  tiles.emplace_back(tileSpriteSheet->makeSprite(payload.position));
}
