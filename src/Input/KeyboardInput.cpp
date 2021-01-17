#include <algorithm>
#include "App/abort.h"
#include "App/Logger.h"
#include "Input/KeyboardInput.h"

bool FW::KeyboardInput::created{false};

FW::KeyboardInput::KeyboardInput() {
  if (created) {
    Logger::error("Cannot create more than 1 KeyboardInput");
    abort();
  }
  created = true;

  int length;
  SDL_GetKeyboardState(&length);
  prevState.resize(length, 0);
  nextState.resize(length, 0);
}

FW::KeyboardInput::~KeyboardInput() {
  created = false;
}

void FW::KeyboardInput::update() {
  SDL_PumpEvents();
  std::copy(nextState.begin(), nextState.end(), prevState.begin());
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  std::copy(state, state + nextState.size(), nextState.begin());
}

bool FW::KeyboardInput::isPrevPressed(Key key) const {
  return prevState[static_cast<std::size_t>(key)];
}

bool FW::KeyboardInput::isPressed(Key key) const {
  return nextState[static_cast<std::size_t>(key)];
}

bool FW::KeyboardInput::isClicked(Key key) const {
  return !isPrevPressed(key) && isPressed(key);
}

bool FW::KeyboardInput::isReleased(Key key) const {
  return isPrevPressed(key) && !isPressed(key);
}
