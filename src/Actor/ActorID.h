#ifndef __ACTOR_ID_H__
#define __ACTOR_ID_H__

#include <cstdint>

namespace FW {
  class ActorID {
  public:
    using Index = std::uint32_t;
    using Version = std::uint8_t;

    ActorID(std::uint32_t index):
      index{index},
      version{0}
    {
    }

    bool operator==(const ActorID& other) const {
      return index == other.index && version == other.version;
    }

    void incrementVersion() {
      ++version;
    }

  private:
    std::uint32_t index : 24;
    std::uint32_t version : 8;
  };
}

#endif
