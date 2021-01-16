#ifndef __EVENT_H__
#define __EVENT_H__

#include "Event/BaseEvent.h"

namespace FW {
  template <typename Payload>
  struct Event: public BaseEvent {
    static EventType type() {
      static EventType result = getNextEventType();
      return result;
    }

    Event(const Payload& payload):
      payload{payload}
    {
    }

    Payload payload;
  };
}
#endif
