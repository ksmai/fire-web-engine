#include "App/abort.h"
#include "App/Clock.h"
#include "App/Logger.h"

bool FW::Clock::created{false};

FW::Clock::Clock() {
  if (created) {
    Logger::error("Cannot create more than 1 Clock");
    abort();
  }
  created = true;
  if (SDL_InitSubSystem(SDL_INIT_TIMER)) {
    Logger::error("SDL_InitSubSystem(SDL_INIT_TIMER) failed: %s", SDL_GetError());
    abort();
  }
  prevCounter = 0;
  counter = SDL_GetPerformanceCounter();
}

FW::Clock::~Clock() {
  SDL_QuitSubSystem(SDL_INIT_TIMER);
  created = false;
}

void FW::Clock::update() {
  prevCounter = counter;
  counter = SDL_GetPerformanceCounter();
}

double FW::Clock::dt() const {
  return counterToMS(counter - prevCounter);
}

double FW::Clock::time() const {
  return counterToMS(counter);
}

double FW::Clock::counterToMS(Counter counter) const {
  return counter * 1000.0 / SDL_GetPerformanceFrequency();
}
