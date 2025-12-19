//
// Created by SamHerts on 5/27/2025.
//

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "System.h"
#include "Vec2.h"
#include "Components.h"


class RenderSystem : public System
{
    Entity mCamera{};


public:
    void Init(Coordinator* coordinator, SDL_Renderer* renderer)
    {

        mCamera = coordinator->CreateEntity();

        coordinator->AddComponent(
            mCamera,
            Object{
                .position = Vec2(0.0f, 0.0f)
            });

        coordinator->AddComponent(
            mCamera,
            Camera{
                .renderer = renderer,
            }
            );
    }

    void RenderShape(Coordinator* coordinator, Entity entity) const
    {
        const auto renderer = coordinator->GetComponent<Camera>(mCamera).renderer;
        const auto position = coordinator->GetComponent<Object>(entity).position;
        const auto& renderable = coordinator->GetComponent<Renderable>(entity);


        const auto [type, size] = coordinator->GetComponent<Shape>(entity);

        SDL_SetRenderScale(renderer, renderable.scale.x, renderable.scale.y);
        SDL_SetRenderDrawColor(renderer, renderable.color.r, renderable.color.g, renderable.color.b, renderable.color.a);
        switch (type)
        {
            case ShapeType::Point:
                SDL_RenderPoint(renderer, position.x, position.y);
                break;
            case ShapeType::Line:
                SDL_RenderLine(renderer, position.x, position.y, size.line_end.x, size.line_end.y);
                break;
            case ShapeType::Rectangle:
                {
                    const SDL_FRect rect = {
                        position.x - renderable.bounding_box.w / 2, position.y - renderable.bounding_box.h / 2,
                        renderable.bounding_box.w, renderable.bounding_box.h};
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                }
            case ShapeType::Circle:
                break;
            case ShapeType::Triangle:
                {
                    SDL_Vertex vertices[3];
                    vertices[0].position = size.triangle.a;
                    vertices[1].position = size.triangle.b;
                    vertices[2].position = size.triangle.c;
                    SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);
                    break;
                }
            default:
                break;
        }
    }

    void RenderText(Coordinator* coordinator, Entity entity) const
    {
        const auto renderer = coordinator->GetComponent<Camera>(mCamera).renderer;
        const auto position = coordinator->GetComponent<Object>(entity).position;
        const auto& renderable = coordinator->GetComponent<Renderable>(entity);

        const auto text = coordinator->GetComponent<Text>(entity).text;

        SDL_SetRenderScale(renderer, renderable.scale.x, renderable.scale.y);
        SDL_SetRenderDrawColor(renderer, renderable.color.r, renderable.color.g, renderable.color.b, renderable.color.a);
        SDL_RenderDebugText(renderer, position.x, position.y, text.c_str());
    }

    void RenderImage(Coordinator* coordinator, Entity entity) const
    {
        const auto renderer = coordinator->GetComponent<Camera>(mCamera).renderer;
        const auto position = coordinator->GetComponent<Object>(entity).position;
        const auto& renderable = coordinator->GetComponent<Renderable>(entity);
        const auto texture = coordinator->GetComponent<Image>(entity).texture;
        float image_width = texture->w;
        float image_height = texture->h;

        SDL_FRect drect {
            .x = position.x - image_width / 2.0f,
            .y = position.y - image_height / 2.0f,
            .w = image_width,
            .h = image_height
        };

        SDL_SetRenderScale(renderer, renderable.scale.x, renderable.scale.y);
        SDL_SetRenderDrawColor(renderer, renderable.color.r, renderable.color.g, renderable.color.b, renderable.color.a);
        SDL_RenderTexture(renderer, texture, nullptr, &drect);
    }

    void grid_to_world(Coordinator* coordinator, Entity entity) const
    {
        const auto renderer = coordinator->GetComponent<Camera>(mCamera).renderer;
        int w, h;
        SDL_GetRenderOutputSize(renderer, &w, &h);
        const auto grid = coordinator->GetComponent<Grid_Local>(entity);

        const float cell_size = w / static_cast<float>(grid.grid_size);

        // --- 3. Calculate the world position ---
        // We add 0.5f to the local position to get the *center* of the cell.
        // If we didn't, we would get the top-left corner of the cell.
        Vec2 world_position;
        world_position.x = (grid.local_position.x + 0.5f) * cell_size;
        world_position.y = (grid.local_position.y + 0.5f) * cell_size;

        auto& object = coordinator->GetComponent<Object>(entity);
        object.position = world_position;
    }

    void Update(Coordinator* coordinator, float dt) const
    {
        std::vector<std::pair<const Renderable*,Entity>> renderable_entities;
        renderable_entities.reserve(mEntities.size());

        // Z Depth Sorting
        for (auto const& entity : mEntities)
        {
            auto const& renderable = coordinator->GetComponent<Renderable>(entity);
            renderable_entities.emplace_back(&renderable, entity);
        }

        std::sort(renderable_entities.begin(), renderable_entities.end(),
        [](const auto& a, const auto& b) {
            return Renderable::compare()(*a.first, *b.first);
            }
        );

        for (const auto&[renderable, entity] : renderable_entities)
        {
            if (coordinator->HasComponent<Grid_Local>(entity))
            {
                grid_to_world(coordinator, entity);
            }

            if (coordinator->HasComponent<Image>(entity))
            {
                RenderImage(coordinator, entity);
            }
            if (coordinator->HasComponent<Shape>(entity))
            {
                RenderShape(coordinator, entity);
            }
            if (coordinator->HasComponent<Text>(entity))
            {
                RenderText(coordinator, entity);
            }
        }
    }
};


#endif //RENDERSYSTEM_H
