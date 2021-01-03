#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <SDL/SDL.h>

namespace FW {
  class Clock {
  public:
    Clock();

    double dt() const;

    void update();

  private:
    Uint64 prevCounter;
    Uint64 counter;
  };
}
#endif
