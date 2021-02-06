#include "Level/LevelManager.h"
#include <iostream>

FW::LevelManager::LevelManager(const std::string& url):
  parser{url},
  initialized{false}
{
}

void FW::LevelManager::update(double dt) {
  if (!initialized) {
    if (parser.isReady()) {
      levels = parser.parse();
      parser.close();
      initialized = true;
    }
    return;
  }
  std::cout << "Level Manager can update now: " << dt << "\n";
}
