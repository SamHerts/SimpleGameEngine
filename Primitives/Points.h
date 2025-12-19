//
// Created by SamHerts on 5/23/2025.
//

#ifndef POINTS_H
#define POINTS_H

#include "utils.h"
#include "../SDL_Includes.h"


class Point {
protected:
    SDL_Color color{};
    SDL_FPoint position{};
    SDL_FPoint velocity{};
    SDL_FPoint acceleration{};
    bool debug = false;
public:
    Point() = default;

    explicit Point(const SDL_Color color) : color(color) {}
    explicit Point(const SDL_Color color, const SDL_FPoint pos) : color(color), position(pos) {}
    explicit Point(const SDL_FPoint position) : position(position) {}

    void setPosition(const SDL_FPoint position) {this->position = position;}
    void setPosition(const float x, const float y) {this->position = {x, y};}
    SDL_FPoint getPosition() const {return position;}

    void setVelocity(const SDL_FPoint velocity) {this->velocity = velocity;}
    void setVelocity(const float velocity_x, const float velocity_y) {this->velocity = {velocity_x, velocity_y}; }
    SDL_FPoint getVelocity() const {return velocity;}

    void setAcceleration(const SDL_FPoint acceleration){this->acceleration = acceleration;}
    void setAcceleration(const float accel_x, const float accel_y){this->acceleration = {accel_x, accel_y};}
    SDL_FPoint getAcceleration() const {return acceleration;}

    void setColor(const SDL_Color color) {this->color = color;}
    SDL_Color getColor() const {return this->color;}

    void update();
    void draw(SDL_Renderer* renderer) const;
    void set_debug(const bool debug) {this->debug = debug;}
};

#endif //POINTS_H
