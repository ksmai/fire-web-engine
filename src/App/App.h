#ifndef __APP_H__
#define __APP_H__

#include <string>
#include <SDL/SDL.h>
#include "Resource/ResourceCache.h"
#include "Process/ProcessManager.h"
#include "App/Clock.h"

// for temp testing
#include <GLES3/gl3.h>
#include "Process/DelayProcess.h"
#include "Graphics/SpriteShader.h"
#include "Graphics/Texture.h"
#include "memory"

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

    ~App();

    App(const App&) =delete;

    App& operator=(const App&) =delete;

    void init();

    void update();

  private:
    SDL_Window* window;
    SDL_GLContext context;

    // ResourceCache should come first so any resources it owned are destroyed last
    ResourceCache resourceCache;
    Clock clock;
    ProcessManager processManager;
    bool initialized{false};

    // for temp testing
    std::unique_ptr<Texture> texture;
    std::unique_ptr<SpriteShader> program;
  };
}

#endif
