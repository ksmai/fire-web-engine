#include "App/abort.h"
#include "App/App.h"
#include "App/Logger.h"

bool FW::App::created{false};

FW::App::App(const Config& config):
  graphics{config.title, config.canvasWidth, config.canvasHeight}
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
}

FW::App::~App() {
  SDL_Quit();
  created = false;
}

void FW::App::update() {
  clock.update();
  keyboardInput.update();
  mouseInput.update();

  std::cout << "App updating so empty\n";
}
