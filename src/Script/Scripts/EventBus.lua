function initEventBus()
  EventBus = EventBus or {}

  local customListeners = {}
  local customListenerIDs = {}

  local addEvent = function(event)
    customListeners[event] = customListeners[event] or {}
  end

  local nextID = function(event)
    customListenerIDs[event] = customListenerIDs[event] or 1
    local id = tostring(customListenerIDs[event])
    customListenerIDs[event] = customListenerIDs[event] + 1
    return id
  end

  EventBus.emitCustom = function(event, data)
    addEvent(event)
    for id, listener in pairs(customListeners[event]) do
      listener(data)
    end
  end

  EventBus.subscribeCustom = function(event, listener)
    addEvent(event)
    local id = nextID(event)
    customListeners[event][id] = listener
    return id
  end

  EventBus.unsubscribeCustom = function(event, id)
    addEvent(event)
    table.remove(customListeners[event], id)
  end
end

initEventBus()
