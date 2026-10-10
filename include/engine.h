#pragma once

#include "window.h"
#include <stdbool.h>

typedef struct {
    Window* window;
    bool is_running;
} Engine;

Engine* engine_init(const char* title, int width, int height, SDL_WindowFlags flags);
void engine_run(Engine* engine);
void engine_shutdown(Engine* engine);
