#ifndef __APP_H__
#define __APP_H__

#include "App/AppConfig.h"
#include "App/Clock.h"
#include "Audio/Audio.h"
#include "Event/EventBus.h"
#include "File/RemoteFile.h"
#include "Game/Game.h"
#include "Graphics/Graphics.h"
#include "Input/KeyboardInput.h"
#include "Input/MouseInput.h"
#include "Process/ProcessRunner.h"
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
    void init();

    static bool created;

    ScriptManager scriptManager;
    Graphics graphics;
    Audio audio;
    KeyboardInput keyboardInput;
    MouseInput mouseInput;
    Clock clock;
    EventBus eventBus;
    ProcessRunner processRunner;

    Game game;

    RemoteFile resourceFile;
    bool initialized;
  };
}

#endif
