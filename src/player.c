#include "player.h"
#include "asset_loader.h"
#include <math.h>
#include <stdlib.h>
#include <SDL3/SDL_log.h>

static const char* SPRITE_PATHS[PLAYER_STATE_COUNT][DIR_COUNT] = {
    [PLAYER_IDLE] = {
        [DIR_DOWN]  = "../assets/Sprites/IDLE/idle_down.png",
        [DIR_LEFT]  = "../assets/Sprites/IDLE/idle_left.png",
        [DIR_RIGHT] = "../assets/Sprites/IDLE/idle_right.png",
        [DIR_UP]    = "../assets/Sprites/IDLE/idle_up.png",
    },
    [PLAYER_RUN] = {
        [DIR_DOWN]  = "../assets/Sprites/RUN/run_down.png",
        [DIR_LEFT]  = "../assets/Sprites/RUN/run_left.png",
        [DIR_RIGHT] = "../assets/Sprites/RUN/run_right.png",
        [DIR_UP]    = "../assets/Sprites/RUN/run_up.png",
    },
};

static const float ANIM_FPS[PLAYER_STATE_COUNT] = {
    [PLAYER_IDLE] = 6.0f,
    [PLAYER_RUN]  = 10.0f,
};

void player_destroy(Player* p) {
    if (p == NULL) return;
    for (int s = 0; s < PLAYER_STATE_COUNT; s++) {
        for (int d = 0; d < DIR_COUNT; d++) {
            if (p->textures[s][d] != NULL) {
                SDL_DestroyTexture(p->textures[s][d]);
            }
        }
    }
    free(p);
}

Player* player_create(SDL_Renderer* renderer, float x, float y) {
    Player* p = (Player*)calloc(1, sizeof(Player));
    if (p == NULL) {
        SDL_Log("Failed to allocate memory for Player");
        return NULL;
    }

    for (int s = 0; s < PLAYER_STATE_COUNT; s++) {
        for (int d = 0; d < DIR_COUNT; d++) {
            SDL_Texture* tex = load_texture(renderer, SPRITE_PATHS[s][d]);
            if (tex == NULL) {
                player_destroy(p);
                return NULL;
            }
            SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
            p->textures[s][d] = tex;
        }
    }

    p->x = x;
    p->y = y;
    p->speed = 260.0f;
    p->state = PLAYER_IDLE;
    p->dir = DIR_DOWN;
    return p;
}

void player_update(Player* p, const Input* input, float dt, int bounds_w, int bounds_h) {
    float mx = input->move_x;
    float my = input->move_y;
    float len = sqrtf(mx * mx + my * my);

    PlayerState next_state = len > 0.0f ? PLAYER_RUN : PLAYER_IDLE;
    if (next_state != p->state) {
        p->state = next_state;
        p->current_frame = 0;
        p->anim_timer = 0.0f;
    }

    if (len > 0.0f) {
        p->x += mx * p->speed * dt;
        p->y += my * p->speed * dt;

        if (fabsf(mx) > fabsf(my)) {
            p->dir = mx < 0.0f ? DIR_LEFT : DIR_RIGHT;
        } else {
            p->dir = my < 0.0f ? DIR_UP : DIR_DOWN;
        }
    }

    float cx = bounds_w * 0.5f;
    float cy = bounds_h * 0.53f;
    float rx = bounds_w * 0.46f;
    float ry = bounds_h * 0.36f;
    float dx = (p->x - cx) / rx;
    float dy = (p->y - cy) / ry;
    float d2 = dx * dx + dy * dy;
    if (d2 > 1.0f) {
        float d = sqrtf(d2);
        p->x = cx + (dx / d) * rx;
        p->y = cy + (dy / d) * ry;
    }

    p->anim_timer += dt;
    float frame_time = 1.0f / ANIM_FPS[p->state];
    while (p->anim_timer >= frame_time) {
        p->anim_timer -= frame_time;
        p->current_frame = (p->current_frame + 1) % PLAYER_FRAMES;
    }
}

void player_render(const Player* p, SDL_Renderer* renderer) {
    SDL_Texture* tex = p->textures[p->state][p->dir];

    float tex_w = 0.0f;
    float tex_h = 0.0f;
    SDL_GetTextureSize(tex, &tex_w, &tex_h);

    float frame_w = tex_w / PLAYER_FRAMES;

    SDL_FRect src = { p->current_frame * frame_w, 0.0f, frame_w, tex_h };

    float w = frame_w * PLAYER_SCALE;
    float h = tex_h * PLAYER_SCALE;
    SDL_FRect dst = { p->x - w * 0.5f, p->y - h * PLAYER_FOOT_Y, w, h };

    SDL_RenderTexture(renderer, tex, &src, &dst);
}