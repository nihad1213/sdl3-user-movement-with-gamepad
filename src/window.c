#include "window.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <stdbool.h>

static bool init_window_and_renderer(Window* game, const char *title, int width, int height, 
    SDL_WindowFlags flags);

Window* create_window(const char *title, int width, int height, SDL_WindowFlags flags, const char* bg_path) {
    Window* game = (Window*)malloc(sizeof(Window));
    
    if (game == NULL) {
        perror("Failed to allocate memory for Window");
        return NULL;
    }

    game->width = width;
    game->height = height;
    game->background = NULL;

    if (!init_window_and_renderer(game, title, width, height, flags)) {
        free(game);
        return NULL;
    }

    if (bg_path != NULL) {
        game->background = IMG_LoadTexture(game->renderer, bg_path);
        if (game->background == NULL) {
            SDL_Log("Failed to load background image (%s): %s", bg_path, SDL_GetError());
        }
    }

    return game;
}

void destroy_window(Window* game) {
    if (game != NULL) {
        if (game->renderer) {
            SDL_DestroyRenderer(game->renderer);
        }

        if (game->background) {
            SDL_DestroyTexture(game->background);
        }
        
        if (game->window) {
            SDL_DestroyWindow(game->window);
        }

        free(game);
    }
}

void render_background(Window* game) {
    if (game->background != NULL) {
        SDL_RenderTexture(game->renderer, game->background, NULL, NULL);
    }
}

static bool init_window_and_renderer(Window* game, const char *title, int width, int height, 
    SDL_WindowFlags flags) {
    
    if (!SDL_CreateWindowAndRenderer(title, width, height, flags, &game->window, &game->renderer)) {
        SDL_Log("Error creating window and renderer: %s", SDL_GetError());
        return false;
    }
    return true;
}