#pragma once

#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "input.h"

#define PLAYER_FRAMES 8
#define PLAYER_SCALE 2.0f
#define PLAYER_FOOT_Y 0.72f

typedef enum {
    PLAYER_IDLE,
    PLAYER_RUN,
    PLAYER_STATE_COUNT
} PlayerState;

typedef enum {
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_COUNT
} PlayerDir;

typedef struct {
    SDL_Texture* textures[PLAYER_STATE_COUNT][DIR_COUNT];

    float x;
    float y;
    float speed;

    PlayerState state;
    PlayerDir dir;
    int current_frame;
    float anim_timer;
} Player;

Player* player_create(SDL_Renderer* renderer, float x, float y);
void player_update(Player* player, const Input* input, float dt, int bounds_w, int bounds_h);
void player_render(const Player* player, SDL_Renderer* renderer);
void player_destroy(Player* player);