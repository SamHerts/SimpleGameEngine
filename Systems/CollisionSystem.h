//
// Created by SamHerts on 6/13/2025.
//

#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "Systems.h"
#include "Vec2.h"
#include "Components.h"


class CollisionSystem : public System
{

public:
    void Init(Coordinator* coordinator)
    {

    }


    void Update(Coordinator* coordinator, float dt) const
    {
        for (auto const& entity : mEntities)
        {
            auto const& [collider, local] = coordinator->GetComponent<Collision>(entity);

            if (local)
            {
                if (coordinator->HasComponent<Object>(entity))
                {

                }
            }
            else
            {

            }
        }
    }
};

#endif //COLLISIONSYSTEM_H
