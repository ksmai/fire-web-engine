#include "App/abort.h"
#include "App/App.h"
#include "App/Logger.h"

bool FW::App::created{false};

FW::App::App(const AppConfig& config):
  graphics{config.title, config.canvasWidth, config.canvasHeight},
  levelManager{config.levelConfig}
{
  if (created) {
    Logger::error("Cannot create more than 1 App");
    abort();
  }
  created = true;
}

FW::App::~App() {
  created = false;
}

void FW::App::update() {
  clock.update();
  keyboardInput.update();
  mouseInput.update();
  levelManager.update(clock.dt());
}
