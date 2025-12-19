//
// Created by SamHerts on 5/27/2025.
//

#ifndef PLAYERCONTROLSYSTEM_H
#define PLAYERCONTROLSYSTEM_H

#include "System.h"
#include "Coordinator.h"
#include "Components.h"


class Event;


class PlayerControlSystem : public System
{
    std::bitset<6> mButtons;

public:
    void Init(Coordinator* coordinator)
    {
        coordinator->AddEventListener(METHOD_LISTENER(Events::Window::INPUT, PlayerControlSystem::InputListener));
    }

    void Update(Coordinator* coordinator, float dt) {
        for (auto const& entity : mEntities) {
            auto& movable = coordinator->GetComponent<Movable>(entity);

            // Reset velocity
            movable.velocity = Vec2(0, 0);

            // Handle movement
            if (mButtons[static_cast<size_t>(InputButtons::W)]) {
                movable.velocity.y = -1;
            }
            if (mButtons[static_cast<size_t>(InputButtons::A)]) {
                movable.velocity.x = -1;
            }
            if (mButtons[static_cast<size_t>(InputButtons::S)]) {
                movable.velocity.y = 1;
            }
            if (mButtons[static_cast<size_t>(InputButtons::D)]) {
                movable.velocity.x = 1;
            }
        }
    }

    void InputListener(Event& event)
    {
        if (event.GetType() == Events::Window::INPUT)
        {
            const auto&[button_ID, value] = event.GetParam<std::pair<InputButtons, bool>>(Events::Window::Input::INPUT);
            mButtons[static_cast<size_t>(button_ID)] = value;
        }
    }
};


#endif //PLAYERCONTROLSYSTEM_H
