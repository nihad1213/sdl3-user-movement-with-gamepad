#pragma once

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* background;
    int width;
    int height;
} Window;

Window* create_window(const char *title, int width, int height, SDL_WindowFlags flags, const char* bg_path);
void destroy_window(Window* app_window);
void render_background(Window* game);