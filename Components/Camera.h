//
// Created by SamHerts on 5/27/2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "../SDL_Includes.h"

struct Camera {
    Sint64 z_index{};
    SDL_FRect view{};
    SDL_FRect viewport{};
    SDL_Renderer* renderer;

    struct compare {
        bool operator()(const Camera& lhs, const Camera& rhs) const {
            return lhs.z_index > rhs.z_index;
        }
    };
};

#endif //CAMERA_H
