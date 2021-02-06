#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include <string>

namespace FW {
  struct AppConfig {
    std::string levelConfig;
    std::string title;
    std::size_t canvasWidth;
    std::size_t canvasHeight;
  };
}
#endif
