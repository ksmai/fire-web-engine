#include "SDL_image.h"
#include "App/App.h"
#include "Resource/TextLoader.h"
#include "Resource/RawLoader.h"
#include "Resource/ImageLoader.h"

// for temp testing
#include <cmath>
#include "Graphics/Shader.h"
#include "Graphics/Transform.h"

FW::App::App(const Config& config) {
  resourceCache.addLoader(new TextLoader{});
  resourceCache.addLoader(new ImageLoader{});
  resourceCache.addLoader(new RawLoader{});
  resourceCache.loadZipFile(config.sharedResourceFile);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    throw "SDL_Init failed";
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  window = SDL_CreateWindow(
    config.title.c_str(),
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    config.canvasWidth,
    config.canvasHeight,
    SDL_WINDOW_OPENGL
  );
  if (window == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
    throw "SDL_CreateWindow failed";
  }

  context = SDL_GL_CreateContext(window);
  if (context == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gl context: %s", SDL_GetError());
    throw "SDL_GL_CreateContext failed";
  }

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GL Version = %d", glGetString(GL_VERSION));
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GLSL Version = %d", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

FW::App::~App() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void FW::App::init() {
  if (initialized || resourceCache.isLoading()) {
    return;
  }

  // for temp testing
  // texture
  Resource* img = resourceCache.getResource("demo/roguelikeSheet_transparent.png");
  spriteSheet.reset(new SpriteSheet{Texture{*img}, 16, 16, 1});
  sprite.reset(new Sprite{2, 1, 3, 2});

  const unsigned char* vertexShaderSource = resourceCache.getResource("demo/SpriteVertex.glsl")->buffer();
  const unsigned char* fragmentShaderSource = resourceCache.getResource("demo/SpriteFragment.glsl")->buffer();
  Shader vertexShader{Shader::createVertexShader(vertexShaderSource)};
  Shader fragmentShader{Shader::createFragmentShader(fragmentShaderSource)};
  program.reset(new SpriteShader{vertexShader, fragmentShader});

  Process::StrongPtr parentProcess{new DelayProcess{3000.0}};
  Process::StrongPtr childProcess{new DelayProcess{2000.0}};
  parentProcess->attachChild(std::move(childProcess));
  processManager.attachProcess(std::move(parentProcess));

  initialized = true;
}

void FW::App::update() {
  if (!initialized) {
    init();
    return;
  }
  clock.update();
  double dt = clock.dt();

  processManager.update(dt);

  // for temp testing
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  Transform t;
  t.translate(-0.2f, -0.2f);
  program->prepareDraw();
  spriteSheet->prepareDraw();
  program->draw(*spriteSheet, *sprite, t);
  spriteSheet->finishDraw();
  program->finishDraw();
}
