#include "App/App.h"

// for temp testing
#include "App/Logger.h"
#include "App/abort.h"
#include "File/ZipFile.h"
#include "File/ImageFile.h"
#include "Graphics/Transform.h"
#include "Graphics/Color.h"
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
  graphics.setClearColor(FW::Color{0.7f, 0.42f, 0.66f, 1.0f});
  ZipFile zipFile{remoteFile.getData()};
  remoteFile.close();
  spriteSheet.reset(new SpriteSheet{zipFile.getFileContent("demo/roguelikeSheet_transparent.png"), 16, 16, 1});
  SpritePosition pos{13, 12, 4, 4};
  sprite.reset(new Sprite{spriteSheet->makeSprite(pos)});

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
  graphics.prepareDraw();
  graphics.prepareDrawSprite();
  spriteSheet->prepareDraw();
  graphics.drawSprite(*sprite);
  spriteSheet->finishDraw();
  graphics.finishDrawSprite();
}
