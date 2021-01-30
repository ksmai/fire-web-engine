#ifndef __EVENT_BUS__
#define __EVENT_BUS__

#include <algorithm>
#include <iterator>
#include <vector>
#include <functional>
#include "lua-5.4.2/src/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "App/abort.h"
#include "App/Logger.h"
#include "Event/Event.h"

namespace FW {
  class EventBus {
  public:
    template <typename T>
    using EventListener = std::function<void(const T&)>;

    using EventListenerID = std::size_t;

    template <typename Payload>
    EventListenerID subscribe(EventListener<Payload> listener) {
      EventType type = Event<Payload>::type();
      ensureSize(type);
      listeners[type].push_back(EventListenerAdapter{listener});
      EventListenerID id{getNextEventListenerID()};
      listenerIDs[type].push_back(id);
      return id;
    }

    template <typename Payload>
    EventListenerID luaSubscribe(luabridge::LuaRef listener) {
      if (!listener.isFunction()) {
        Logger::error("Subscribing with a non-function LuaRef");
        abort();
      }
      EventType type = Event<Payload>::type();
      ensureSize(type);
      listeners[type].push_back(LuaEventListenerAdapter<Payload>{listener});
      EventListenerID id{getNextEventListenerID()};
      listenerIDs[type].push_back(id);
      return id;
    }

    template <typename Payload>
    void unsubscribe(EventListenerID id) {
      EventType type = Event<Payload>::type();
      ensureSize(type);
      auto it{std::find(listenerIDs[type].begin(), listenerIDs[type].end(), id)};
      if (it == listenerIDs[type].end()) {
        return;
      }
      auto index{std::distance(listenerIDs[type].begin(), it)};
      listeners[type].erase(listeners[type].begin() + index);
      listenerIDs[type].erase(it);
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
        listenerIDs.resize(size + 1);
      }
    }

    EventListenerID getNextEventListenerID() {
      return nextID++;
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

    template <typename Payload>
    struct LuaEventListenerAdapter {
      LuaEventListenerAdapter(luabridge::LuaRef listener):
        listener{listener}
      {
      }

      void operator()(const BaseEvent& event) {
        listener(static_cast<const Event<Payload>&>(event).payload);
      }

      luabridge::LuaRef listener;
    };

    std::vector<std::vector<EventListener<BaseEvent>>> listeners;
    std::vector<std::vector<EventListenerID>> listenerIDs;
    EventListenerID nextID{0};
  };
}

#endif
