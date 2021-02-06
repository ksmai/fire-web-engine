#ifndef __APP_H__
#define __APP_H__

#include <string>
#include "App/Clock.h"
#include "Audio/Audio.h"
#include "Graphics/Graphics.h"
#include "Input/KeyboardInput.h"
#include "Input/MouseInput.h"
#include "Script/ScriptManager.h"

namespace FW {
  class App {
  public:
    struct Config {
      std::string sharedResourceFile;
      std::string title;
      std::size_t canvasWidth;
      std::size_t canvasHeight;
    };

    explicit App(const Config& config);
    App(const App&) =delete;
    App& operator=(const App&) =delete;
    ~App();

    void update();

  private:
    static bool created;

    ScriptManager scriptManager;
    Graphics graphics;
    Audio audio;
    KeyboardInput keyboardInput;
    MouseInput mouseInput;
    Clock clock;
  };
}

#endif
