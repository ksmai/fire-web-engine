#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <string>
#include "SDL/SDL.h"
#include "File/Data.h"

namespace FW {
  class Graphics {
  public:
    Graphics(const std::string& title, std::size_t canvasWidth, std::size_t canvasHeight);
    Graphics(const Graphics&) =delete;
    Graphics& operator=(const Graphics&) =delete;
    ~Graphics();

  private:
    SDL_Window* window;
    SDL_GLContext context;
  };
}
#endif
