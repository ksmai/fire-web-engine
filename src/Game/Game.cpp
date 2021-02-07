#include "File/XMLFile.h"
#include "Game/Game.h"

#include "Render/AddTilePosition.h"

void FW::Game::init(
  ScriptManager* scriptManager,
  Graphics* graphics,
  Audio* audio,
  KeyboardInput* keyboardInput,
  MouseInput* mouseInput,
  EventBus* eventBus,
  ProcessRunner* processRunner,
  ZipFile&& resources
) {
  this->scriptManager = scriptManager;
  this->audio = audio;
  this->keyboardInput = keyboardInput;
  this->mouseInput = mouseInput;
  this->processRunner = processRunner;

  renderer.reset(new Renderer{eventBus, graphics});
  renderer->init(resources);

  renderer->setTileSheet(0);
}

void FW::Game::update(double dt, double time) {
  renderer->update(dt, time);
}
