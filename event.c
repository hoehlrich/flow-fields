#include "flowfields.h"

void updatemousepos(SDL_Context *ctx, SDL_Event *event, SDL_Point *mousepos) {
    int w, h;
    SDL_GetWindowSize(ctx->window, &w, &h);
    mousepos->x = event->motion.x;
    mousepos->y = event->motion.y;
}

void handlekeyevent(SDL_Context *ctx, State *state, SDL_Event *event) {
    switch (event->key.keysym.sym) {
        case SDLK_s:
            save_flowfield(ctx);
            break;
        case SDLK_1:
            state->activetexture = ctx->flowfield;
            break;
        case SDLK_2:
            state->activetexture = ctx->perlin;
            break;
        case SDLK_3:
            state->activetexture = ctx->vectorfield;
            break;
    }
}

void handleevent(SDL_Context *ctx, State *state, SDL_Event *event) {
    switch (event->type) {
        case SDL_QUIT:
            cleanup(ctx, state);
            break;
        case SDL_MOUSEMOTION:
            updatemousepos(ctx, event, state->mousepos);
            break;
        case SDL_MOUSEBUTTONDOWN:
            rendercurve(ctx, state, state->mousepos->x, state->mousepos->y);
            break;
        case SDL_KEYDOWN:
            handlekeyevent(ctx, state, event);
            break;
    }
}



void handleevents(SDL_Context *ctx, State *state) {
    SDL_Event *event = malloc(sizeof(SDL_Event));
    while (SDL_PollEvent(event)) {
        handleevent(ctx, state, event);
    }
    free(event);
}

