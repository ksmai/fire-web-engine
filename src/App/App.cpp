#include "App/abort.h"
#include "App/App.h"
#include "App/Logger.h"
#include "File/ZipFile.h"

bool FW::App::created{false};

FW::App::App(const AppConfig& config):
  graphics{config.title, config.canvasWidth, config.canvasHeight},
  resourceFile{config.resources},
  initialized{false}
{
  if (created) {
    Logger::error("Cannot create more than 1 App");
    abort();
  }
  created = true;
  resourceFile.open();
}

FW::App::~App() {
  created = false;
}

void FW::App::update() {
  if (!initialized) {
    init();
    return;
  }
  clock.update();
  game.update(clock.dt(), clock.time());
}

void FW::App::init() {
  if (resourceFile.isError()) {
    Logger::error("Failed to load resource file: %s", resourceFile.getError().c_str());
    abort();
  }
  if (resourceFile.isOpened()) {
    game.init(
      &scriptManager,
      &graphics,
      &audio,
      &keyboardInput,
      &mouseInput,
      &eventBus,
      &processRunner,
      ZipFile{resourceFile.getData()}
    );
    resourceFile.close();
    initialized = true;
  }
}
