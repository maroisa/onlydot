#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL_scancode.h>
#include <stdio.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>


typedef struct {
    SDL_Window  *window;
    SDL_Renderer *renderer;
    SDL_FRect dot;
    SDL_FColor dot_color;
    Uint64 last_time;

} AppState;


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    if (!SDL_Init(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;

    AppState *app = SDL_calloc(1, sizeof(AppState));
    if (!app) return SDL_APP_FAILURE;
    *appstate = app;

    if (!SDL_CreateWindowAndRenderer("OnlyDot", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &app->window, &app->renderer)) {
        return SDL_APP_FAILURE;
    }

    app->dot = (SDL_FRect){WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT / 2 - 10, 20, 20};
    app->dot_color = (SDL_FColor){1,1,1};

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    if (event->type == SDL_EVENT_QUIT){
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate){
    AppState *app = (AppState*)appstate;

    Uint64 current_time = SDL_GetTicks();
    float dt = (current_time - app->last_time) / 1000.0f;
    app->last_time = current_time;

    const bool *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A]) app->dot.x -= 0.05;
    if (keys[SDL_SCANCODE_D]) app->dot.x += 0.05;
    if (keys[SDL_SCANCODE_S]) app->dot.y += 0.05;
    if (keys[SDL_SCANCODE_W]) app->dot.y -= 0.05;
    if (keys[SDL_SCANCODE_SPACE]) {
        app->dot.h += 0.05;
        app->dot.y -= 0.025;
        app->dot.w += 0.05;
        app->dot.x -= 0.025;

        if (app->dot.h >= 500) {
            app->dot.x = WINDOW_WIDTH / 2;
            app->dot.y = WINDOW_HEIGHT / 2;
            app->dot.h = 10;
            app->dot.w = 10;

            SDL_FColor newColor = {SDL_randf(), SDL_randf(), SDL_randf()};
            while (newColor.r <= 0.5 && newColor.g <= 0.5 && newColor.b <= 0.5) {
                newColor = (SDL_FColor){SDL_randf(), SDL_randf(), SDL_randf()};
            }

            app->dot_color = newColor;
        }
    }


    SDL_SetRenderDrawColor(app->renderer, 50, 50, 50, 255);
    SDL_RenderClear(app->renderer);

    SDL_SetRenderDrawColorFloat(app->renderer, app->dot_color.r, app->dot_color.g, app->dot_color.b, 1);
    SDL_RenderFillRect(app->renderer, &app->dot);

    SDL_RenderPresent(app->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){
    if (appstate) {
        AppState* app = (AppState*)appstate;
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        SDL_free(app);
    }
}
