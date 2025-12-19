//
// Created by SamHerts on 5/27/2025.
//

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Points.h"


class Rectangle : public Point {
private:
    SDL_FRect _rect{};
protected:
    float width{};
    float height{};

public:
    Rectangle() = default;
    Rectangle(SDL_FPoint center, float width, float height);
    void setWidth(const float width);
    void setHeight(const float height);
    void draw(SDL_Renderer* renderer);
};



#endif //RECTANGLE_H
