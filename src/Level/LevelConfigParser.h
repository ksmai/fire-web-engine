#ifndef __LEVEL_CONFIG_PARSER_H__
#define __LEVEL_CONFIG_PARSER_H__

#include <string>
#include <vector>
#include "File/RemoteFile.h"
#include "Level/LevelConfig.h"

namespace FW {
  class LevelConfigParser {
  public:
    explicit LevelConfigParser(const std::string& url);
    ~LevelConfigParser() =default;

    bool isReady() const;
    std::vector<LevelConfig> parse() const;
    void close();

  private:
    RemoteFile configFile;
  };
}
#endif
