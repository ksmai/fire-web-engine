#ifndef __APP_H__
#define __APP_H__

#include <string>
#include "App/Clock.h"
#include "Graphics/Graphics.h"

// for temp testing
#include "Event/EventBus.h"
#include "Process/ProcessRunner.h"
#include "File/RemoteFile.h"
#include "Process/DelayProcess.h"
#include "Graphics/SpriteSheet.h"
#include "Graphics/Sprite.h"
#include <memory>

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

    void init();

    void update();

  private:
    static bool created;

    Graphics graphics;
    EventBus eventBus;
    RemoteFile remoteFile;
    Clock clock;
    ProcessRunner processRunner;
    bool initialized{false};

    // for temp testing
    std::unique_ptr<SpriteSheet> spriteSheet;
    std::unique_ptr<Sprite> sprite;
  };
}

#endif
