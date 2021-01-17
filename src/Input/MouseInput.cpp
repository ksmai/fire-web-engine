#include "App/abort.h"
#include "App/Logger.h"
#include "Input/MouseInput.h"

bool FW::MouseInput::created{false};

FW::MouseInput::MouseInput() {
  if (created) {
    Logger::error("Cannot create more than 1 MouseInput");
    abort();
  }
  created = true;
  x = y = 0;
  prevLeft = nextLeft = false;
  prevMiddle = nextMiddle = false;
  prevRight = nextRight = false;
}

FW::MouseInput::~MouseInput() {
  created = false;
}

void FW::MouseInput::update() {
  prevLeft = nextLeft;
  prevMiddle = nextMiddle;
  prevRight = nextRight;
  SDL_PumpEvents();
  Uint32 state = SDL_GetMouseState(&x, &y);
  nextLeft = (state & SDL_BUTTON(SDL_BUTTON_LEFT));
  nextMiddle = (state & SDL_BUTTON(SDL_BUTTON_MIDDLE));
  nextRight = (state & SDL_BUTTON(SDL_BUTTON_RIGHT));
}

bool FW::MouseInput::isPrevPressed(Button button) const {
  switch (button) {
  case Button::LEFT:
    return prevLeft;
  case Button::MIDDLE:
    return prevMiddle;
  case Button::RIGHT:
    return prevRight;
  }
}

bool FW::MouseInput::isPressed(Button button) const {
  switch (button) {
  case Button::LEFT:
    return nextLeft;
  case Button::MIDDLE:
    return nextMiddle;
  case Button::RIGHT:
    return nextRight;
  }
}

bool FW::MouseInput::isClicked(Button button) const {
  return !isPrevPressed(button) && isPressed(button);
}

bool FW::MouseInput::isReleased(Button button) const {
  return isPrevPressed(button) && !isPressed(button);
}

int FW::MouseInput::getX() const {
  return x;
}

int FW::MouseInput::getY() const {
  return y;
}
