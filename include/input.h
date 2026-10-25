#pragma once

#include <SDL3/SDL.h>

typedef struct {
    SDL_Gamepad* gamepad;
    float move_x;
    float move_y;
} Input;

void input_init(Input* input);
void input_handle_event(Input* input, const SDL_Event* event);
void input_update(Input* input);
void input_shutdown(Input* input);