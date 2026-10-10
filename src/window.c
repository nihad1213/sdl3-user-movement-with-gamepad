#include "window.h"
#include <stdio.h>
#include <stdlib.h>

Window* create_window(const char *title, int width, int height, SDL_WindowFlags flags) {
    Window* game = (Window*)malloc(sizeof(Window));
    
    if (game == NULL) {
        perror("Failed to allocate memory for Window");
        return NULL;
    }

    game->width = width;
    game->height = height;

    if (!init_window_and_renderer(game, title, width, height, flags)) {
        free(game);
        return NULL;
    }

}

void destroy_window(Window* game) {
    if (game != NULL) {
        if (game->renderer) {
            SDL_DestroyRenderer(game->renderer);
        }
        
        if (game->window) {
            SDL_DestroyWindow(game->window);
        }

        free(game);
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