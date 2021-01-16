#include "Event/BaseEvent.h"

FW::EventType FW::BaseEvent::getNextEventType() {
  static EventType eventType = 0;
  return eventType++;
}
