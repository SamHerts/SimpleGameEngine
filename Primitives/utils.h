//
// Created by SamHerts on 5/23/2025.
//

#ifndef UTILS_H
#define UTILS_H
#include <cmath>
#include <string>

#include "../SDL_Includes.h"



constexpr float TEXT_POS_X = 10.0f; // X position for the debug text
constexpr float TEXT_POS_Y = 10.0f; // Y position for the debug text

class FPS_Tool
{
    Uint64 frameCount{0};
    Uint64 lastFPSUpdateTick{SDL_GetPerformanceCounter()};
    Uint64 performanceFrequency = SDL_GetPerformanceFrequency();
    Uint64 targetFPSTicks{};
    Uint64 frameStartTick{};
    float currentFPS{};
    const double fpsUpdateInterval{0.5}; // update FPS display every 1 second

public:
    int targetFPS{};

    explicit FPS_Tool(int target_fps)
        : targetFPS(target_fps), targetFPSTicks(performanceFrequency / target_fps) {}

    void tick()
    {
        ++frameCount;
        frameStartTick = SDL_GetPerformanceCounter();
    }

    void update()
    {
        const Uint64 frameEndTick = SDL_GetPerformanceCounter();

        if (targetFPS >= 0)
        {
            const Uint64 elapsedTicks = frameEndTick - frameStartTick;

            // Frame delay to cap FPS
            if (elapsedTicks < targetFPSTicks)
            {
                const Uint32 delayMs = static_cast<Uint32>(
                    (static_cast<double>(targetFPSTicks - elapsedTicks) / performanceFrequency) * 1000.0
                );
                SDL_Delay(delayMs);
            }
        }

        // Update FPS counter
        const double elapsedTime = static_cast<double>(frameEndTick - lastFPSUpdateTick) / performanceFrequency;
        if (elapsedTime >= fpsUpdateInterval)
        {
            currentFPS = static_cast<float>(frameCount / elapsedTime);
            frameCount = 0;
            lastFPSUpdateTick = frameEndTick;
        }
    }


    void draw(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDebugTextFormat(renderer, TEXT_POS_X, TEXT_POS_Y, "FPS: %.1f", currentFPS);
    }
};


inline SDL_Color mapVelocityToRGB(const SDL_FPoint velocity) {
    // 1. Calculate the magnitude (speed) of the velocity vector.
    const auto speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    // 2. Determine the maximum possible speed within the given input range [0,0] to [1,1].
    // This occurs at (1,1), so MAX_SPEED = sqrt(1^2 + 1^2) = sqrt(2).
    constexpr auto MAX_SPEED = 1.41421356;

    // 3. Normalize the speed to a [0.0, 1.0] range.
    // Normalized speed will be 0 for (0,0) and 1 for (1,1).
    double normalized_speed = speed / MAX_SPEED;

    // Clamp normalized_speed to ensure it stays within [0.0, 1.0]
    // This handles potential floating point inaccuracies or if inputs were slightly outside [0,1]
    normalized_speed = std::max(0.0, std::min(1.0, normalized_speed));

    // 4. Calculate the RGB components based on the normalized speed.
    SDL_Color color{};

    // Red component:
    // Red is the color for highest velocity, and white (lowest velocity) also has full red.
    // So, the red component will always be 255.
    color.r = 255;

    // Green and Blue components:
    // They should go from 255 (for white, i.e., normalized_speed = 0)
    // down to 0 (for red, i.e., normalized_speed = 1).
    // So, a value of 1.0 - normalized_speed will be 1.0 at low speed and 0.0 at high speed.
    int green_blue_value = (1.0 - normalized_speed) * 255;
    green_blue_value = std::max(0, std::min(255, green_blue_value));
    color.g = green_blue_value;
    color.b = green_blue_value;

    return color;
}

inline SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string&path)
{
    const std::string texturePath = std::string(SDL_GetBasePath()) + "Assets/" + path;
    SDL_Log("Texture path: %s\n", texturePath.c_str());

    auto png_surface = IMG_Load(texturePath.c_str());
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, png_surface);
    SDL_DestroySurface(png_surface);

    return tex;
}

#endif //UTILS_H
