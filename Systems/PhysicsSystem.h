//
// Created by SamHerts on 5/27/2025.
//

#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "System.h"
#include "Coordinator.h"
#include "Components.h"

class PhysicsSystem : public System
{
public:
    void Init(){}

    void Update(Coordinator* coordinator, float dt)
    {
        for (auto const& entity : mEntities)
        {
            auto& movable = coordinator->GetComponent<Movable>(entity);
            auto& object = coordinator->GetComponent<Object>(entity);

            // Forces
            auto const& gravity = coordinator->GetComponent<Gravity>(entity);

            if (coordinator->HasComponent<Grid_Local>(entity))
            {
                auto& grid = coordinator->GetComponent<Grid_Local>(entity);
                auto& timer = coordinator->GetComponent<IntervalMove>(entity);

                // 1. Accumulate the time since the last frame
                timer.timer += dt;

                // 2. Check if enough time has passed
                // We use a 'while' loop in case the game lags and multiple moves
                // should happen in a single frame.
                while (timer.timer >= timer.move_interval)
                {
                    // 3. Perform the discrete move
                    grid.local_position += movable.velocity;

                    // 4. Subtract the interval from the accumulator.
                    // This is more accurate than setting to 0, as it preserves
                    // any leftover time.
                    timer.timer -= timer.timer;
                }
            }
            else
            {
                object.position += movable.velocity * dt;
            }

            movable.velocity += gravity.force * dt;
        }
    }
};


#endif //PHYSICSSYSTEM_H
