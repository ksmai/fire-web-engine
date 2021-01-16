#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <string>
#include <memory>
#include "SDL/SDL.h"
#include "File/Data.h"
#include "Graphics/Sprite.h"
#include "Graphics/SpriteShader.h"

namespace FW {
  class Graphics {
  public:
    Graphics(const std::string& title, std::size_t canvasWidth, std::size_t canvasHeight);
    Graphics(const Graphics&) =delete;
    Graphics& operator=(const Graphics&) =delete;
    ~Graphics();

    void prepareDraw() const;

    void prepareDrawSprite() const;
    void drawSprite(const Sprite&) const;
    void finishDrawSprite() const;

  private:
    std::unique_ptr<SpriteShader> spriteShader;
    SDL_Window* window;
    SDL_GLContext context;
  };
}
#endif
