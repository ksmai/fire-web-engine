#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <vector>
#include "Event/EventBus.h"
#include "File/ZipFile.h"
#include "Graphics/Graphics.h"
#include "Render/AddTilePosition.h"
#include "Render/SpriteSheetRepository.h"

namespace FW {
  class Renderer {
  public:
    explicit Renderer(EventBus* eventBus, Graphics* graphics);
    Renderer(const Renderer&) =delete;
    Renderer& operator=(const Renderer&) =delete;
    ~Renderer();

    void init(ZipFile& resources);
    void update(double dt, double time);
    void setTileSheet(SpriteSheetRepository::ID);
    void handleAddTilePosition(const AddTilePosition&);

  private:
    EventBus* eventBus;
    Graphics* graphics;
    SpriteSheetRepository spriteSheetRepository;

    const SpriteSheet* tileSpriteSheet;
    std::vector<Sprite> tiles;
    EventBus::EventListenerID addTilePositionListenerID;
  };
}
#endif
