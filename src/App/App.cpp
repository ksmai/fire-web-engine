#include "App/abort.h"
#include "App/App.h"
#include "App/Logger.h"

// for temp testing
#include "File/ZipFile.h"
#include "File/ImageFile.h"
#include "Graphics/Transform.h"
#include "Graphics/Color.h"
#include "Actor/ActorID.h"
#include <iostream>
#include <functional>

bool FW::App::created{false};

struct MyData1 {
  int x, y;
};

struct MyData2 {
  std::string secret;
};

void myListener1(const MyData1& d) {
  std::cout << "myListener1 called with " << d.x << " " << d.y << "\n";
}

void myListener2(const MyData2& d) {
  std::cout << "myListener2 called with " << d.secret << "\n";
}

void myListener3(const MyData1& d) {
  std::cout << "myListener3 called with " << d.x << " " << d.y << "\n";
}

struct MyListenerObject {
  void myListenMethod(const MyData2& d) {
    std::cout << "MyListenerObject::myListenMethod called with " << d.secret << "\n";
  }
};

MyListenerObject obj;

FW::App::App(const Config& config):
  graphics{config.title, config.canvasWidth, config.canvasHeight},
  remoteFile{"/resources.zip"}
{
  if (created) {
    Logger::error("Cannot create more than 1 App");
    abort();
  }
  created = true;
  if (SDL_Init(0)) {
    Logger::error("SDL_Init() failed: %s", SDL_GetError());
    abort();
  }

  remoteFile.open();
}

FW::App::~App() {
  SDL_Quit();
  created = false;
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
  processRunner.attachProcess(std::move(parentProcess));

  ActorID actorID{0}, actorID2{0};
  SDL_Log("sizeof(actorID) = %d", sizeof(actorID));
  SDL_Log("actorID==actorID2 = %d", actorID==actorID2);
  actorID.incrementVersion();
  SDL_Log("actorID==actorID2 = %d", actorID==actorID2);

  eventBus.subscribe<MyData2>(std::bind(&MyListenerObject::myListenMethod, &obj, std::placeholders::_1));
  eventBus.emit<MyData2>({"NotCalled"});
  eventBus.emit<MyData1>({1, 2});
  auto id1 = eventBus.subscribe<MyData1>(myListener1);
  eventBus.emit<MyData2>({"NotCalled2"});
  eventBus.emit<MyData1>({3, 4});
  eventBus.subscribe<MyData2>(myListener2);
  eventBus.emit<MyData2>({"Called!!"});
  eventBus.emit<MyData1>({5, 6});
  auto id2 = eventBus.subscribe<MyData1>(myListener3);
  eventBus.emit<MyData2>({"Called Again!!"});
  eventBus.unsubscribe<MyData1>(id1);
  eventBus.unsubscribe<MyData1>(id2);
  eventBus.unsubscribe<MyData1>(id2);
  eventBus.unsubscribe<MyData2>(id2);
  eventBus.emit<MyData1>({7, 8});

  initialized = true;
}

void FW::App::update() {
  if (!initialized) {
    init();
    return;
  }
  clock.update();
  double dt = clock.dt();

  processRunner.update(dt);

  // for temp testing
  graphics.prepareDraw();
  graphics.prepareDrawSprite();
  spriteSheet->prepareDraw();
  graphics.drawSprite(*sprite);
  spriteSheet->finishDraw();
  graphics.finishDrawSprite();
}
