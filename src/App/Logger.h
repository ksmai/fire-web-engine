#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "SDL/SDL.h"

namespace FW {
  class Logger {
  public:
    template <typename... Args>
    static void info(const char* format, Args... args) {
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, format, args...);
    }

    template <typename ...Args>
    static void error(const char* format, Args... args) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, format, args...);
    }
  };
}

#endif
