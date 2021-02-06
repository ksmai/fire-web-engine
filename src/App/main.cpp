#include "emscripten.h"
#include "App/App.h"
#include "App/AppConfig.h"

FW::App* app;

void loop(void* arg) {
  FW::App* app = static_cast<FW::App*>(arg);
  app->update();
}

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void start(const char* configFilePath, const char* title, std::size_t canvasWidth, std::size_t canvasHeight) {
    FW::AppConfig config;
    config.title = title;
    config.canvasWidth = canvasWidth;
    config.canvasHeight = canvasHeight;
    config.configFilePath = configFilePath;
    app = new FW::App{config};
    emscripten_set_main_loop_arg(loop, app, 0, false);
    // TODO clean up after game is done?
  }

  EMSCRIPTEN_KEEPALIVE
  void stop() {
    if (app) {
      emscripten_cancel_main_loop();
      delete app;
      app = nullptr;
    }
  }
}
