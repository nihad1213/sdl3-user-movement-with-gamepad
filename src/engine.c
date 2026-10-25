#include "engine.h"
#include "asset_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>

Engine* engine_init(const char* title, int width, int height, SDL_WindowFlags flags, const char* bg_path) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return NULL;
    }

    Engine* engine = (Engine*)calloc(1, sizeof(Engine));
    if (engine == NULL) {
        perror("Failed to allocate memory for Engine");
        SDL_Quit();
        return NULL;
    }

    engine->window = create_window(title, width, height, flags, bg_path);
    if (engine->window == NULL) {
        free(engine);
        SDL_Quit();
        return NULL;
    }

    if (engine->window->background == NULL) {
        SDL_Log("Warning: Proceeding without background asset.");
    }

    input_init(&engine->input);

    engine->player = player_create(engine->window->renderer, width * 0.5f, height * 0.65f);
    if (engine->player == NULL) {
        SDL_Log("Warning: Proceeding without player.");
    }

    engine->is_running = true;
    return engine;
}

void engine_run(Engine* engine) {
    SDL_Event event;
    Uint64 last = SDL_GetTicks();

    while (engine->is_running) {
        Uint64 now = SDL_GetTicks();
        float dt = (now - last) / 1000.0f;
        last = now;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                engine->is_running = false;
            }
            input_handle_event(&engine->input, &event);
        }

        input_update(&engine->input);

        int out_w = 0, out_h = 0;
        SDL_GetRenderOutputSize(engine->window->renderer, &out_w, &out_h);

        if (engine->player != NULL) {
            player_update(engine->player, &engine->input, dt, out_w, out_h);
        }

        SDL_RenderClear(engine->window->renderer);
        render_background(engine->window);
        if (engine->player != NULL) {
            player_render(engine->player, engine->window->renderer);
        }
        SDL_RenderPresent(engine->window->renderer);
    }
}

void engine_shutdown(Engine* engine) {
    if (engine != NULL) {
        if (engine->player != NULL) {
            player_destroy(engine->player);
        }
        input_shutdown(&engine->input);
        if (engine->window != NULL) {
            destroy_window(engine->window);
        }
        free(engine);
    }
    SDL_Quit();
}