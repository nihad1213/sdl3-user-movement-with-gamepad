#include "engine.h"
#include <SDL3/SDL_video.h>

int main() {

    Engine* engine = engine_init("Arena Game",  1280, 720, SDL_WINDOW_RESIZABLE);
    if (engine == NULL) {
        return 1;
    }

    engine_run(engine);

    engine_shutdown(engine);

    return 0;
}