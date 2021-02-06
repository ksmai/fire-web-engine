#include "App/abort.h"
#include "App/App.h"
#include "App/Logger.h"

bool FW::App::created{false};

FW::App::App(const AppConfig& config):
  graphics{config.title, config.canvasWidth, config.canvasHeight},
  configFile{config.configFilePath},
  isInitializing{true},
  isLoadingLevel{false}
{
  if (created) {
    Logger::error("Cannot create more than 1 App");
    abort();
  }
  created = true;
  if (SDL_Init(0)) {
    Logger::error("SDL_Init() failed: %s", SDL_GetError());
    abort();
  }

  configFile.open();
}

FW::App::~App() {
  SDL_Quit();
  created = false;
}

void FW::App::update() {
  if (isInitializing) {
    if (configFile.isOpened()) {
      Logger::info("Loaded");
      isInitializing = false;
    } else if (configFile.isError()) {
      Logger::error("Config file cannot be opened: %s", configFile.getError().c_str());
      abort();
    } else {
      return;
    }
  }

  if (isLoadingLevel) {
    Logger::info("loading level");
    return;
  }

  clock.update();
  keyboardInput.update();
  mouseInput.update();

  std::cout << "App updating so empty\n";
}
