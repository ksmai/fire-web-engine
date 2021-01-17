#include "Random/RNG.h"

FW::RNG::RNG():
  RNG{std::random_device{}()}
{
}

FW::RNG::RNG(Seed seed):
  gen{seed},
  seed{seed}
{
}

bool FW::RNG::prob(double p) {
  return std::bernoulli_distribution{p}(gen);
}

FW::RNG::Seed FW::RNG::getSeed() const {
  return seed;
}
