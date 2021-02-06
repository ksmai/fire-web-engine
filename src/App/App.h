#ifndef __APP_H__
#define __APP_H__

#include "App/AppConfig.h"
#include "App/Clock.h"
#include "Audio/Audio.h"
#include "Graphics/Graphics.h"
#include "Input/KeyboardInput.h"
#include "Input/MouseInput.h"
#include "Level/LevelManager.h"
#include "Script/ScriptManager.h"

namespace FW {
  class App {
  public:
    explicit App(const AppConfig& config);
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
    LevelManager levelManager;
  };
}

#endif
