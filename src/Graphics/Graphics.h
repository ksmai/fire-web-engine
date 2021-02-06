#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <string>
#include <memory>
#include "SDL/SDL.h"
#include "File/Data.h"
#include "Graphics/Color.h"
#include "Graphics/Sprite.h"
#include "Graphics/SpriteShader.h"
#include "Graphics/SpriteSheet.h"

namespace FW {
  class Graphics {
  public:
    Graphics(const std::string& title, std::size_t canvasWidth, std::size_t canvasHeight);
    Graphics(const Graphics&) =delete;
    Graphics& operator=(const Graphics&) =delete;
    ~Graphics();

    void setClearColor(const Color&);

    void prepareDraw() const;

    SpriteSheet makeSpriteSheet(Data&&, unsigned int spriteWidth, unsigned int spriteHeight, unsigned int margin = 0) const;
    void prepareDrawSprite() const;
    void drawSprite(const Sprite&) const;
    void finishDrawSprite() const;
    void finishDraw() const;

  private:
    static bool created;
    Color clearColor;
    std::unique_ptr<SpriteShader> spriteShader;
    SDL_Window* window;
    SDL_GLContext context;
  };
}
#endif
