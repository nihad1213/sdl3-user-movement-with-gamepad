#include "input.h"
#include <math.h>

#define STICK_DEADZONE 0.25f

void input_init(Input* input) {
    input->gamepad = NULL;
    input->move_x = 0.0f;
    input->move_y = 0.0f;

    int count = 0;
    SDL_JoystickID* ids = SDL_GetGamepads(&count);
    if (ids != NULL) {
        if (count > 0) {
            input->gamepad = SDL_OpenGamepad(ids[0]);
            if (input->gamepad == NULL) {
                SDL_Log("Failed to open gamepad: %s", SDL_GetError());
            }
        }
        SDL_free(ids);
    }
}

void input_handle_event(Input* input, const SDL_Event* event) {
    if (event->type == SDL_EVENT_GAMEPAD_ADDED) {
        if (input->gamepad == NULL) {
            input->gamepad = SDL_OpenGamepad(event->gdevice.which);
            if (input->gamepad == NULL) {
                SDL_Log("Failed to open gamepad: %s", SDL_GetError());
            }
        }
    } else if (event->type == SDL_EVENT_GAMEPAD_REMOVED) {
        if (input->gamepad != NULL && SDL_GetGamepadID(input->gamepad) == event->gdevice.which) {
            SDL_CloseGamepad(input->gamepad);
            input->gamepad = NULL;
        }
    }
}

void input_update(Input* input) {
    const bool* keys = SDL_GetKeyboardState(NULL);

    float kx = 0.0f;
    float ky = 0.0f;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  kx -= 1.0f;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) kx += 1.0f;
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    ky -= 1.0f;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  ky += 1.0f;

    float gx = 0.0f;
    float gy = 0.0f;
    if (input->gamepad != NULL) {
        float ax = SDL_GetGamepadAxis(input->gamepad, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f;
        float ay = SDL_GetGamepadAxis(input->gamepad, SDL_GAMEPAD_AXIS_LEFTY) / 32767.0f;
        float mag = sqrtf(ax * ax + ay * ay);

        if (mag > STICK_DEADZONE) {
            float scaled = (mag - STICK_DEADZONE) / (1.0f - STICK_DEADZONE);
            if (scaled > 1.0f) scaled = 1.0f;
            gx = (ax / mag) * scaled;
            gy = (ay / mag) * scaled;
        }

        if (SDL_GetGamepadButton(input->gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT))  gx = -1.0f;
        if (SDL_GetGamepadButton(input->gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT)) gx = 1.0f;
        if (SDL_GetGamepadButton(input->gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP))    gy = -1.0f;
        if (SDL_GetGamepadButton(input->gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN))  gy = 1.0f;
    }

    float x = (kx != 0.0f || ky != 0.0f) ? kx : gx;
    float y = (kx != 0.0f || ky != 0.0f) ? ky : gy;

    float len = sqrtf(x * x + y * y);
    if (len > 1.0f) {
        x /= len;
        y /= len;
    }

    input->move_x = x;
    input->move_y = y;
}

void input_shutdown(Input* input) {
    if (input->gamepad != NULL) {
        SDL_CloseGamepad(input->gamepad);
        input->gamepad = NULL;
    }
}