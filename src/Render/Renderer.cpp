#include <functional>
#include "Render/Renderer.h"

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
  graphics->prepareDrawSprite();
  tileSpriteSheet->prepareDraw();
  for (Sprite& sprite : tiles) {
    graphics->drawSprite(sprite);
  }
  tileSpriteSheet->finishDraw();
  graphics->finishDrawSprite();
  graphics->finishDraw();
}

void FW::Renderer::handleAddTilePosition(const AddTilePosition& payload) {
  if (!tileSpriteSheet) {
    return;
  }
  tiles.emplace_back(tileSpriteSheet->makeSprite(payload.position));
}
