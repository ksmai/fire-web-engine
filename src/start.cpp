#include <iostream>
#include <emscripten.h>
#include <SDL.h>

class Game {
public:
  Game():
    currentTimestamp{emscripten_get_now()}
  {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }

    window = SDL_CreateWindow(
      "MySDLWindowTitle",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_WINDOW_OPENGL
    );
    if (window == nullptr) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
    }

    surface = SDL_CreateRGBSurface(0, 512, 512, 32, 0, 0, 0, 0);
  }

  void update() {
    lastTimestamp = currentTimestamp;
    currentTimestamp = emscripten_get_now();
    std::cout << "dt = " << (currentTimestamp - lastTimestamp) << "\n";

    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    Uint8* pixels = static_cast<Uint8*>(surface->pixels);
        for (int i=0; i < 1048576; i++) {
        char randomByte = rand() % 255;
        pixels[i] = randomByte;
    }

    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

     SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(screenTexture);
  }

private:
  double lastTimestamp, currentTimestamp;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Surface* surface;
};

void loop(void* arg) {
  Game* gameLogic = static_cast<Game*>(arg);
  gameLogic->update();
}

extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void start() {
    emscripten_set_main_loop_arg(loop, new Game{}, 0, false);
  }
}
