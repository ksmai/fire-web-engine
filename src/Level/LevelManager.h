#ifndef __LEVEL_MANAGER_H__
#define __LEVEL_MANAGER_H__

#include <string>
#include <vector>
#include "Level/Level.h"
#include "Level/LevelConfig.h"
#include "Level/LevelConfigParser.h"

namespace FW {
  class LevelManager {
  public:
    explicit LevelManager(const std::string& url);
    LevelManager(const LevelManager&) =delete;
    LevelManager& operator=(const LevelManager&) =delete;
    ~LevelManager() =default;

    void update(double dt);

  private:
    void startLevel(std::size_t id);

    LevelConfigParser parser;
    Level currentLevel;
    std::vector<LevelConfig> levels;
    bool initialized;
  };
}
#endif
