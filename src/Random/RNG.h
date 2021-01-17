#ifndef __RNG_H__
#define __RNG_H__

#include <random>

namespace FW {
  class RNG {
  public:
    using Seed = std::mt19937::result_type;

    RNG();
    RNG(Seed);

    bool prob(double);
    Seed getSeed() const;

  private:
    std::mt19937 gen;
    Seed seed;
  };
}

#endif
