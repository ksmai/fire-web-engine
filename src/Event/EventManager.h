#ifndef __EVENT_MANAGER__
#define __EVENT_MANAGER__

#include <vector>
#include <functional>
#include "Event/Event.h"

namespace FW {
  class EventManager {
  public:
    template <typename T>
    using EventListener = std::function<void(const T&)>;

    template <typename Payload>
    void subscribe(EventListener<Payload> listener) {
      EventType type = Event<Payload>::type();
      ensureSize(type);
      listeners[type].push_back(EventListenerAdapter{listener});
    }

    template <typename Payload>
    void emit(const Payload& payload) {
      EventType type = Event<Payload>::type();
      ensureSize(type);
      Event<Payload> event{payload};
      for (auto& listener: listeners[type]) {
        listener(event);
      }
    }


  private:
    void ensureSize(std::size_t size) {
      if (size >= listeners.size()) {
        listeners.resize(size + 1);
      }
    }

    template <typename Payload>
    struct EventListenerAdapter {
      EventListenerAdapter(EventListener<Payload> listener):
        listener{listener}
      {
      }

      void operator()(const BaseEvent& event) {
        listener(static_cast<const Event<Payload>&>(event).payload);
      }

      EventListener<Payload> listener;
    };

    std::vector<std::vector<EventListener<BaseEvent>>> listeners;
  };
}

#endif
