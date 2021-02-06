#include "App/abort.h"
#include "App/Logger.h"
#include "File/XMLFile.h"
#include "Level/LevelConfigParser.h"

FW::LevelConfigParser::LevelConfigParser(const std::string& url):
  configFile{url}
{
  configFile.open();
}

bool FW::LevelConfigParser::isReady() const {
  if (configFile.isError()) {
    Logger::error("Couldn't load level config file: %s", configFile.getError().c_str());
    abort();
  }
  return configFile.isOpened();
}

std::vector<FW::LevelConfig> FW::LevelConfigParser::parse() const {
  if (!isReady()) {
    return std::vector<LevelConfig>{};
  }
  XMLFile xml{configFile.getData()};
  auto levels = xml.root().find("Level");
  std::vector<LevelConfig> result{levels.size()};
  for (auto& level : levels) {
    std::size_t id{std::stoul(level.attr("id"))};
    if (id >= result.size()) {
      result.resize(id + 1);
    }
    if (!result[id].url.empty()) {
      Logger::error("Duplicated level definition: %d", id);
      abort();
    }
    result[id].url = level.attr("config");
    if (result[id].url.empty()) {
      Logger::error("Missing config url: %d", id);
      abort();
    }
    const char* nextID{level.attr("next")};
    result[id].hasNext = nextID != nullptr;
    if (result[id].hasNext) {
      result[id].nextID = std::stoul(nextID);
    }
  }
  return result;
}

void FW::LevelConfigParser::close() {
  configFile.close();
}
