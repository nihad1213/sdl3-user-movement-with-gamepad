#pragma once

#include "window.h"
#include <stdbool.h>
#include <SDL3_image/SDL_image.h>

typedef struct {
    Window* window;
    SDL_Texture* background_texture;
    bool is_running;
} Engine;

Engine* engine_init(const char* title, int width, int height, SDL_WindowFlags flags);
void engine_run(Engine* engine);
void engine_shutdown(Engine* engine);
