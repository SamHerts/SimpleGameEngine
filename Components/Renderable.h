//
// Created by SamHerts on 5/27/2025.
//

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "../SDL_Includes.h"

struct Renderable {
    SDL_Color color{};
    int depth{};
    SDL_FRect bounding_box{};
    SDL_FPoint scale{1.0,1.0};

    struct compare {
        bool operator()(const Renderable& lhs, const Renderable& rhs) const {
            return lhs.depth < rhs.depth;
        }
    };
};


#endif //RENDERABLE_H
