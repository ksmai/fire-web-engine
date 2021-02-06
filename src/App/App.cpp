#include "App/abort.h"
#include "App/App.h"
#include "App/Logger.h"

bool FW::App::created{false};

FW::App::App(const AppConfig& config):
  graphics{config.title, config.canvasWidth, config.canvasHeight},
  levelConfigParser{config.levelConfig},
  isInitializing{true},
  isLoadingLevel{false}
{
  if (created) {
    Logger::error("Cannot create more than 1 App");
    abort();
  }
  created = true;
}

FW::App::~App() {
  created = false;
}

void FW::App::update() {
  if (isInitializing) {
    if (levelConfigParser.isReady()) {
      Logger::info("ready to parse levels");
      std::vector<LevelConfig> levels{levelConfigParser.parse()};
      std::size_t id{0};
      for (auto& level : levels) {
        std::cout << "DEBUG: found level " << (id++) << " , ";
        if (level.hasNext) {
          std::cout << "next = " << level.nextID << " , ";
        }
        std::cout << level.url << "\n";
      }
      levelConfigParser.close();
      isInitializing = false;
    } else {
      return;
    }
  }

  if (isLoadingLevel) {
    Logger::info("loading level");
    return;
  }

  clock.update();
  keyboardInput.update();
  mouseInput.update();

  std::cout << "App updating so empty\n";
}
