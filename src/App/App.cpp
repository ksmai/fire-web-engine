#include "SDL_image.h"
#include "App/App.h"

// for temp testing
#include <cmath>
#include "App/Logger.h"
#include "App/abort.h"
#include "File/ZipFile.h"
#include "File/ImageFile.h"
#include "Graphics/Transform.h"
#include "Actor/ActorID.h"

FW::App::App(const Config& config) {
  remoteFile.open();

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
  if (initialized) {
    return;
  }
  if (remoteFile.isError()) {
    Logger::error(remoteFile.getError().c_str());
    abort();
  }
  if (!remoteFile.isOpened()) {
    return;
  }

  // for temp testing
  // texture
  ZipFile zipFile{remoteFile.getData()};
  remoteFile.close();
  ImageFile imageFile{zipFile.getFileContent("demo/roguelikeSheet_transparent.png")};
  spriteSheet.reset(new SpriteSheet{Texture{imageFile}, 16, 16, 1});
  spritePosition.reset(new SpritePosition{13, 7, 3, 2});
  spriteShader.reset(new SpriteShader{});

  Process::StrongPtr parentProcess{new DelayProcess{3000.0}};
  Process::StrongPtr childProcess{new DelayProcess{2000.0}};
  parentProcess->attachChild(std::move(childProcess));
  processManager.attachProcess(std::move(parentProcess));

  ActorID actorID{0}, actorID2{0};
  SDL_Log("sizeof(actorID) = %d", sizeof(actorID));
  SDL_Log("actorID==actorID2 = %d", actorID==actorID2);
  actorID.incrementVersion();
  SDL_Log("actorID==actorID2 = %d", actorID==actorID2);

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
  t.scale(0.5f, 0.5f);
  t.rotate(3.141592654f*1.0f);
  spriteShader->prepareDraw();
  spriteSheet->prepareDraw();
  spriteShader->draw(*spriteSheet, *spritePosition, t);
  spriteSheet->finishDraw();
  spriteShader->finishDraw();
}
