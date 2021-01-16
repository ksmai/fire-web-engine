#include "GLES3/gl3.h"
#include "App/abort.h"
#include "App/Logger.h"
#include "Graphics/Graphics.h"

bool FW::Graphics::created{false};

FW::Graphics::Graphics(const std::string& title, std::size_t canvasWidth, std::size_t canvasHeight) {
  if (created) {
    Logger::error("Cannot create more than 1 Graphics");
    abort();
  }
  created = true;

  if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    Logger::error("SDL_InitSubSystem(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
    abort();
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  window = SDL_CreateWindow(
    title.c_str(),
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    canvasWidth,
    canvasHeight,
    SDL_WINDOW_OPENGL
  );
  if (window == nullptr) {
    Logger::error("Couldn't create window: %s", SDL_GetError());
    abort();
  }

  context = SDL_GL_CreateContext(window);
  if (context == nullptr) {
    Logger::error("Couldn't create gl context: %s", SDL_GetError());
    abort();
  }

  Logger::info("GL Version = %d", glGetString(GL_VERSION));
  Logger::info("GLSL Version = %d", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  spriteShader.reset(new SpriteShader{});
}

FW::Graphics::~Graphics() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  created = false;
}

void FW::Graphics::setClearColor(const Color& color) {
  clearColor = color;
}

void FW::Graphics::prepareDraw() const {
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void FW::Graphics::prepareDrawSprite() const {
  spriteShader->prepareDraw();
}

void FW::Graphics::drawSprite(const Sprite& sprite) const {
  spriteShader->draw(sprite);
}

void FW::Graphics::finishDrawSprite() const {
  spriteShader->finishDraw();
}
