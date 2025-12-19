//
// Created by SamHerts on 5/23/2025.
//

#include "Points.h"

void Point::draw(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderPoint(renderer, position.x, position.y);
    if (debug)
    {
        const auto x_offset = position.x + 5;
        SDL_RenderDebugTextFormat(renderer, x_offset, position.y, "v_x: %.3f", velocity.x);
        SDL_RenderDebugTextFormat(renderer, x_offset, position.y + 10, "v_y: %.3f", velocity.y);
        SDL_RenderDebugTextFormat(renderer, x_offset, position.y + 20, "a_x: %.3f", acceleration.x);
        SDL_RenderDebugTextFormat(renderer, x_offset, position.y + 30, "a_y: %.3f", acceleration.y);
    }
}

void Point::update()
{
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;
    position.x += velocity.x;
    position.y += velocity.y;
}






