#pragma once

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;
} Window;

Window* create_window(const char *title, int width, int height, SDL_WindowFlags flags);
void destroy_window(Window* app_window);