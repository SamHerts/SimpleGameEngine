//
// Created by SamHerts on 5/27/2025.
//

#ifndef TYPES_H
#define TYPES_H

#include <bitset>
#include <cstdint>


// Source: https://gist.github.com/Lee-R/3839813
inline constexpr std::uint32_t fnv1a_32(char const *s, std::size_t count) {
    return count ? (fnv1a_32(s, count - 1) ^ s[count - 1]) * 16777619u : 2166136261u;
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
{
    return fnv1a_32(s, count);
}


// ECS
using Entity = std::uint32_t;
constexpr Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
constexpr ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;


// Input
enum class InputButtons
{
    W = 0,
    A,
    S,
    D,
    Q,
    E
};


// Events
using EventId = std::uint32_t;
using ParamId = std::uint32_t;

#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(EventType, Listener) EventType, std::bind(&Listener, std::placeholders::_1)

// TODO: Make these easier to define and use (macro?)
// TODO: Add some kind of enforcement/automation that a SetParam type and a GetParam type match

namespace Events::Window {
    constexpr EventId QUIT = "Events::Window::QUIT"_hash;
    constexpr EventId RESIZED = "Events::Window::RESIZED"_hash;
    constexpr EventId INPUT = "Events::Window::INPUT"_hash;
}

namespace Events::Window::Input {
    constexpr ParamId INPUT = "Events::Window::Input::INPUT"_hash;
}

namespace Events::Window::Resized {
    constexpr ParamId WIDTH = "Events::Window::Resized::WIDTH"_hash;
    constexpr ParamId HEIGHT = "Events::Window::Resized::HEIGHT"_hash;
}

namespace Events::Physics::Collision {
    constexpr EventId COLLISION = "Events::Physics::Collision::COLLISION"_hash;
}


#endif //TYPES_H
