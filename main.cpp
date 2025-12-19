#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "vendored/imgui/imgui.h"

#include <vector>
#include <filesystem>
#include <memory>
#include <optional>
#include <random>

#include "CollisionSystem.h"
#include "Components.h"
#include "Coordinator.h"
#include "Systems.h"
#include "Math/Vec2.h"

#include "SDL_Includes.h"
#include "Primitives/utils.h"
#include "Primitives/Points.h"
#include "Primitives/Rectangle.h"

constexpr uint32_t windowStartWidth = 1000;
constexpr uint32_t windowStartHeight = 1000;
constexpr uint32_t TARGET_FPS = 30;
constexpr uint32_t GRID_SIZE = 100;

struct AppContext
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_AppResult app_quit = SDL_APP_CONTINUE;
    std::unique_ptr<FPS_Tool> fps_tool;
    Coordinator coordinator{};
    float dt{};
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Ant AI Game", "1.0", "ant.ai.game");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL Minimal Sample",
        windowStartWidth,
        windowStartHeight,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
        );
    if (not window){
        return SDL_APP_FAILURE;
    }

    // create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (not renderer){
        return SDL_APP_FAILURE;
    }

    const std::vector textures = {
        loadTexture(renderer, "trees_bg.png"),
        loadTexture(renderer, "sun_rays.png"),
        loadTexture(renderer, "trees_light.png"),
        loadTexture(renderer, "trees_dark.png"),
    };

    SDL_ShowWindow(window);

    *appstate = new AppContext{
        .window = window,
        .renderer = renderer,
        .fps_tool = std::make_unique<FPS_Tool>(TARGET_FPS),
     };

    AppContext& context = *static_cast<AppContext*>(*appstate);
    context.coordinator.Init();

    // Component Registration
    context.coordinator.RegisterComponent<Camera>();
    context.coordinator.RegisterComponent<Gravity>();
    context.coordinator.RegisterComponent<Renderable>();
    context.coordinator.RegisterComponent<Movable>();
    context.coordinator.RegisterComponent<Object>();
    context.coordinator.RegisterComponent<Shape>();
    context.coordinator.RegisterComponent<Image>();
    context.coordinator.RegisterComponent<Text>();
    context.coordinator.RegisterComponent<Grid_Local>();
    context.coordinator.RegisterComponent<IntervalMove>();
    context.coordinator.RegisterComponent<UserInput>();
    context.coordinator.RegisterComponent<Collision>();

    // Collision System
    auto collision_system = context.coordinator.RegisterSystem<CollisionSystem>();
    {
        Signature signature;
        signature.set(context.coordinator.GetComponentType<Collision>());

        context.coordinator.SetSystemSignature<CollisionSystem>(signature);
    }
    collision_system->Init(&context.coordinator);

    // Physics System
    auto physics_system = context.coordinator.RegisterSystem<PhysicsSystem>();
    {
        Signature signature;
        signature.set(context.coordinator.GetComponentType<Gravity>());
        signature.set(context.coordinator.GetComponentType<Movable>());

        context.coordinator.SetSystemSignature<PhysicsSystem>(signature);
    }
    physics_system->Init();

    // Render System
    auto render_system = context.coordinator.RegisterSystem<RenderSystem>();
    {
        Signature signature;
        signature.set(context.coordinator.GetComponentType<Renderable>());
        signature.set(context.coordinator.GetComponentType<Object>());
        // signature.set(context.coordinator.GetComponentType<Grid_Local>());

        context.coordinator.SetSystemSignature<RenderSystem>(signature);
    }
    render_system->Init(&context.coordinator, renderer);

    // Player Control System
    auto player_control_system = context.coordinator.RegisterSystem<PlayerControlSystem>();
    {
        Signature signature;
        signature.set(context.coordinator.GetComponentType<UserInput>());
        signature.set(context.coordinator.GetComponentType<Movable>());

        context.coordinator.SetSystemSignature<PlayerControlSystem>(signature);
    }
    player_control_system->Init(&context.coordinator);


    const auto ant = context.coordinator.CreateEntity();
    context.coordinator.AddComponents(ant,
        Object{},
        Grid_Local{
            .grid_size = GRID_SIZE,
            .local_position = {0,0}
        },
        Renderable{
            .color = {255, 255, 255, 255},
            .bounding_box = {0, 0, 10, 10}
        },
        Shape{
            .type = ShapeType::Rectangle
        },
        IntervalMove{
            .move_interval = 0.05
        },
        Movable{},
        Gravity{},
        Collision{},
        UserInput{}
    );

    const auto wall = context.coordinator.CreateEntity();
    context.coordinator.AddComponents(wall,
        Object{
        .position = {windowStartWidth - windowStartWidth / 3, windowStartHeight / 2}
        },
        Renderable{
            .color = {0, 00, 255, 255},
            .bounding_box = {0, 0, 10, 200}
        },
        Shape{
            .type = ShapeType::Rectangle
        },
        Collision{}
    );

    SDL_Log("Application started successfully!");

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppContext& context = *static_cast<AppContext*>(appstate);
    switch (event->type)
    {
        case SDL_EVENT_QUIT:
            context.coordinator.SendEvent(Events::Window::QUIT);
            return SDL_APP_SUCCESS;
        case SDL_EVENT_WINDOW_RESIZED:
        {
            Event e(Events::Window::RESIZED);
            e.SetParam(Events::Window::Resized::WIDTH, event->window.data1);
            e.SetParam(Events::Window::Resized::HEIGHT, event->window.data2);
            context.coordinator.SendEvent(e);
            break;
        }
        case SDL_EVENT_KEY_UP:
        case SDL_EVENT_KEY_DOWN:
        {
            std::optional<InputButtons> button;
            switch(event->key.key) {
                case SDLK_W: button = InputButtons::W; break;
                case SDLK_A: button = InputButtons::A; break;
                case SDLK_S: button = InputButtons::S; break;
                case SDLK_D: button = InputButtons::D; break;
                case SDLK_Q: button = InputButtons::Q; break;
                case SDLK_E: button = InputButtons::E; break;
                default: break;
            }

            if (button) {
                Event e(Events::Window::INPUT);
                e.SetParam(Events::Window::Input::INPUT, std::make_pair(*button, event->type == SDL_EVENT_KEY_DOWN));
                context.coordinator.SendEvent(e);
            }
            return SDL_APP_CONTINUE;
        }
        default:
            return SDL_APP_CONTINUE;
    }
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppContext& context = *static_cast<AppContext*>(appstate);
    auto startTime = std::chrono::high_resolution_clock::now();

    int window_width, window_height;
    SDL_GetWindowSize(context.window, &window_width, &window_height);

    SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(context.renderer);  /* start with a blank canvas. */

    context.coordinator.GetSystem<CollisionSystem>()->Update(&context.coordinator, context.dt);
    context.coordinator.GetSystem<PlayerControlSystem>()->Update(&context.coordinator, context.dt);
    context.coordinator.GetSystem<PhysicsSystem>()->Update(&context.coordinator, context.dt);
    context.coordinator.GetSystem<RenderSystem>()->Update(&context.coordinator, context.dt);


    SDL_RenderPresent(context.renderer);
    auto stopTime = std::chrono::high_resolution_clock::now();
    context.dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
