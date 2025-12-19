//
// Created by SamHerts on 5/27/2025.
//

#include "Rectangle.h"


Rectangle::Rectangle(const SDL_FPoint center, const float width, const float height)
{
    position = center;
    this->width = width;
    this->height = height;
}

void Rectangle::setWidth(const float width)
{
    this->width = width;
}

void Rectangle::setHeight(const float height)
{
    this->height = height;
}

void Rectangle::draw(SDL_Renderer* renderer)
{
    _rect.x = position.x - this->width / 2;
    _rect.y = position.y - this->height / 2;
    _rect.w = this->width;
    _rect.h = this->height;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &_rect);

    if (debug)
    {
        SDL_RenderDebugTextFormat(renderer, position.x, position.y, "v_x: %.3f", velocity.x);
        SDL_RenderDebugTextFormat(renderer, position.x, position.y + 10, "v_y: %.3f", velocity.y);
        SDL_RenderDebugTextFormat(renderer, position.x, position.y + 20, "a_x: %.3f", acceleration.x);
        SDL_RenderDebugTextFormat(renderer, position.x, position.y + 30, "a_y: %.3f", acceleration.y);
    }
}
