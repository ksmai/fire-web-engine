#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include <string>

namespace FW {
  struct LevelConfig {
    std::string url;
    std::size_t nextID;
    bool hasNext;
  };
}
#endif
