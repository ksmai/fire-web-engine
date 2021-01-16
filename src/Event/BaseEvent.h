#ifndef __BASE_EVENT_H__
#define __BASE_EVENT_H__

#include <cstddef>

namespace FW {
  using EventType = std::size_t;

  class BaseEvent {
  protected:
    static EventType getNextEventType();
  };
}
#endif
