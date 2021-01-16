#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <SDL/SDL.h>

namespace FW {
  class Clock {
  public:
    using Counter = Uint64;

    Clock();
    ~Clock();

    double dt() const;
    double time() const;
    void update();

  private:
    static bool created;

    double counterToMS(Counter) const;

    Counter prevCounter;
    Counter counter;
  };
}
#endif
