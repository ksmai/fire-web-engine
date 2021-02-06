#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include "App/Clock.h"
#include "Audio/Audio.h"
#include "Event/EventBus.h"
#include "File/ZipFile.h"
#include "Game/SpriteSheetRepository.h"
#include "Graphics/Graphics.h"
#include "Input/KeyboardInput.h"
#include "Input/MouseInput.h"
#include "Process/ProcessRunner.h"
#include "Script/ScriptManager.h"

namespace FW {
  class Game {
  public:
    Game() =default;
    Game(const Game&) =delete;
    Game& operator=(const Game&) =delete;
    ~Game() =default;

    void init(
      ScriptManager* scriptManager,
      Graphics* graphics,
      Audio* audio,
      KeyboardInput* keyboardInput,
      MouseInput* mouseInput,
      Clock* clock,
      EventBus* eventBus,
      ProcessRunner* processRunner,
      ZipFile&& resources
    );

    void update();

  private:
    ScriptManager* scriptManager;
    Graphics* graphics;
    Audio* audio;
    KeyboardInput* keyboardInput;
    MouseInput* mouseInput;
    Clock* clock;
    EventBus* eventBus;
    ProcessRunner* processRunner;

    std::unique_ptr<SpriteSheetRepository> spriteSheetRepository;
  };
}
#endif
