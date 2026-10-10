#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>

static bool load_engine_assets(Engine* engine, const char* image_path);

Engine* engine_init(const char* title, int width, int height, SDL_WindowFlags flags) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return NULL;
    }

    Engine* engine = (Engine*)malloc(sizeof(Engine));
    if (engine == NULL) {
        perror("Failed to allocate memory for Engine");
        SDL_Quit();
        return NULL;
    }

    engine->window = create_window(title, width, height, flags);
    if (engine->window == NULL) {
        free(engine);
        SDL_Quit();
        return NULL;
    }

    if (!load_engine_assets(engine, "../assets/bg.gif")) {
        destroy_window(engine->window);
        free(engine);
        SDL_Quit();
        return NULL;
    }

    engine->is_running = true;
    return engine;
}

void engine_run(Engine* engine) {
    SDL_Event event;

    while (engine->is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                engine->is_running = false;
            }
        }

        // TEST TO SEE WINDOW IN LINUX WILL CHANGE IN FUTURE
        SDL_SetRenderDrawColor(engine->window->renderer, 40, 40, 40, 255);
        SDL_RenderClear(engine->window->renderer);

        SDL_RenderPresent(engine->window->renderer);
    }
}

void engine_shutdown(Engine* engine) {
    if (engine != NULL) {
        if (engine->window != NULL) {
            destroy_window(engine->window);
        }
        free(engine);
    }
    SDL_Quit();
}



static bool load_engine_assets(Engine* engine, const char* image_path) {
    engine->background_texture = IMG_LoadTexture(engine->window->renderer, image_path);
    if (engine->background_texture == NULL) {
        SDL_Log("Failed to load background image: %s", SDL_GetError());
        return false;
    }
    return true;
}