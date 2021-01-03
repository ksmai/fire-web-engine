#include <emscripten.h>
#include "App/App.h"

void loop(void* arg) {
  FW::App* app = static_cast<FW::App*>(arg);
  app->update();
}

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void start() {
    // TODO pass in config with parameters
    FW::App::Config config;
    config.title = "MyConfiguredTitle";
    config.canvasWidth = 1280;
    config.canvasHeight = 720;
    config.sharedResourceFile = "/resources.zip";
    FW::App* app = new FW::App{config};
    emscripten_set_main_loop_arg(loop, app, 0, false);
    // TODO clean up after game is done?
  }
}
