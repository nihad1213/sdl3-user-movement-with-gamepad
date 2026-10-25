#include "asset_loader.h"
#include <SDL3/SDL_log.h>

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path) {
    if (renderer == NULL) {
        SDL_Log("Error: Renderer is NULL, cannot load texture: %s", path);
        return NULL;
    }

    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (texture == NULL) {
        SDL_Log("Failed to load image at '%s': %s", path, SDL_GetError());
        return NULL;
    }

    return texture;
}