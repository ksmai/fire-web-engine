#ifndef __APP_H__
#define __APP_H__

#include <string>
#include "App/Clock.h"
#include "Graphics/Graphics.h"

// for temp testing
#include "Process/ProcessManager.h"
#include "File/RemoteFile.h"
#include "Process/DelayProcess.h"
#include "Graphics/SpriteShader.h"
#include "Graphics/Texture.h"
#include "Graphics/SpriteSheet.h"
#include "Graphics/SpritePosition.h"
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

    ~App() =default;

    App(const App&) =delete;

    App& operator=(const App&) =delete;

    void init();

    void update();

  private:
    Graphics graphics;
    RemoteFile remoteFile;
    Clock clock;
    ProcessManager processManager;
    bool initialized{false};

    // for temp testing
    std::unique_ptr<SpriteSheet> spriteSheet;
    std::unique_ptr<SpritePosition> spritePosition;
    std::unique_ptr<SpriteShader> spriteShader;
  };
}

#endif
