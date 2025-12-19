//
// Created by SamHerts on 5/27/2025.
//

#ifndef SHAPE_H
#define SHAPE_H

#include "Vec2.h"

enum class ShapeType {
    Point,
    Line,
    Triangle,
    Rectangle,
    Circle,
};

struct Triangle_vertexes
{
    SDL_FPoint a;
    SDL_FPoint b;
    SDL_FPoint c;
};

struct Shape {
    ShapeType type;
    union size
    {
        SDL_FPoint line_end;
        Triangle_vertexes triangle;
        float radius;
    } size;
};


#endif //SHAPE_H
