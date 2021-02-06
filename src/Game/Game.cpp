#include "File/XMLFile.h"
#include "Game/Game.h"

void FW::Game::init(
  ScriptManager* scriptManager,
  Graphics* graphics,
  Audio* audio,
  KeyboardInput* keyboardInput,
  MouseInput* mouseInput,
  Clock* clock,
  EventBus* eventBus,
  ProcessRunner* processRunner,
  ZipFile&& resources
) {
  this->scriptManager = scriptManager;
  this->graphics = graphics;
  this->audio = audio;
  this->keyboardInput = keyboardInput;
  this->mouseInput = mouseInput;
  this->clock = clock;
  this->eventBus = eventBus;
  this->processRunner = processRunner;

  spriteSheetRepository.reset(new SpriteSheetRepository{graphics});
  spriteSheetRepository->load(resources);
}

void FW::Game::update() {
  // hardcoded tests
  graphics->prepareDraw();
  graphics->prepareDrawSprite();
  const SpriteSheet& spriteSheet{spriteSheetRepository->get(0)};
  SpritePosition pos{13, 12, 4, 4};
  Sprite sprite{spriteSheet.makeSprite(pos)};
  spriteSheet.prepareDraw();
  graphics->drawSprite(sprite);
  spriteSheet.finishDraw();
  graphics->finishDrawSprite();
  graphics->finishDraw();
}
