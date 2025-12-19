//
// Created by SamHerts on 5/27/2025.
//

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Event.h"
#include "Types.h"
#include <functional>
#include <list>
#include <unordered_map>

#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)


class EventManager
{
    std::unordered_map<EventId, std::list<std::function<void(Event&)>>> listeners;
public:
    void AddListener(EventId eventId, std::function<void(Event&)> const& listener)
    {
        listeners[eventId].push_back(listener);
    }

    void SendEvent(Event& event)
    {
        uint32_t type = event.GetType();

        for (auto const& listener : listeners[type])
        {
            listener(event);
        }
    }

    void SendEvent(EventId eventId)
    {
        Event event(eventId);

        for (auto const& listener : listeners[eventId])
        {
            listener(event);
        }
    }
};


#endif //EVENTMANAGER_H
