#pragma once

#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#define PLAYER_SHEET_COLS 6
#define PLAYER_SHEET_ROWS 6

typedef enum {
    PLAYER_IDLE,
    PLAYER_WALK,
    PLAYER_ATTACK,
    PLAYER_STATE_COUNT
} PlayerState;

typedef struct {
    int row;
    int frames;
    float fps;
    bool loop;
} PlayerAnim;

typedef struct {
    SDL_Texture* texture;
    int frame_w;
    int frame_h;
    float scale;
 
    float x;
    float y;
    float speed;
 
    PlayerState state;
    int current_frame;
    float anim_timer;
    bool facing_left;
 
    float move_x;
    float move_y;
    bool attack_requested;
} Player;

Player* player_create(SDL_Renderer* renderer, const char* sprite_path, float x, float y);
void player_handle_input(Player* player);
void player_update(Player* player, float dt, int bounds_w, int bounds_h);
void player_render(const Player* player, SDL_Renderer* renderer);
void player_destroy(Player* player);