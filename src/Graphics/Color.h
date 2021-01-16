#ifndef __COLOR_H__
#define __COLOR_H__

namespace FW {
  struct Color {
    explicit Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f):
      r{r},
      g{g},
      b{b},
      a{a}
    {
    }

    float r, g, b, a;
  };
}
#endif
