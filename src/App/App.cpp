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

FW::App::App(const Config& config):
  graphics{config.title, config.canvasWidth, config.canvasHeight},
  remoteFile{"/resources.zip"}
{
  remoteFile.open();
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
