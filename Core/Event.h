//
// Created by SamHerts on 5/27/2025.
//

#ifndef EVENT_H
#define EVENT_H

#include "Types.h"
#include <any>
#include <unordered_map>


class Event
{
    EventId mType{};
    std::unordered_map<EventId, std::any> mData{};
public:
    Event() = delete;

    explicit Event(EventId type)
        : mType(type)
    {}

    template<typename T>
    void SetParam(EventId id, T value)
    {
        mData[id] = value;
    }

    template<typename T>
    T GetParam(EventId id)
    {
        return std::any_cast<T>(mData[id]);
    }

    EventId GetType() const
    {
        return mType;
    }
};


#endif //EVENT_H
