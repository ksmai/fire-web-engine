#ifndef __MOUSE_INPUT_H__
#define __MOUSE_INPUT_H__

#include "SDL/SDL.h"

namespace FW {
  class MouseInput {
  public:
    enum class Button {
      LEFT,
      MIDDLE,
      RIGHT
    };

    MouseInput();
    MouseInput(const MouseInput&) =delete;
    MouseInput& operator=(const MouseInput&) =delete;
    ~MouseInput();

    void update();

    bool isPrevPressed(Button button = Button::LEFT) const;
    bool isPressed(Button button = Button::LEFT) const;
    bool isClicked(Button button = Button::LEFT) const;
    bool isReleased(Button button = Button::LEFT) const;

    int getX() const;
    int getY() const;

  private:
    static bool created;

    int x, y;
    bool prevLeft, nextLeft;
    bool prevMiddle, nextMiddle;
    bool prevRight, nextRight;
  };
}
#endif
