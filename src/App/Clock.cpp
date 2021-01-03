#include "App/Clock.h"

FW::Clock::Clock(): prevCounter{0}, counter{SDL_GetPerformanceCounter()} {
}

void FW::Clock::update() {
  prevCounter = counter;
  counter = SDL_GetPerformanceCounter();
}

double FW::Clock::dt() const {
  return (counter - prevCounter) * 1000.0 / SDL_GetPerformanceFrequency();
}
