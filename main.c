#include "flowfields.h"

#define FREQUENCY   0.05
#define DEPTH       8

void generate_perlin(SDL_Context *ctx, State *state) {
    SDL_SetRenderTarget(ctx->renderer, ctx->perlin);
    for (int y = 0; y < IMAGEHEIGHT; y++) {
        for (int x = 0; x < IMAGEWIDTH; x++) {
            int value = 0xFF * perlin(x, y, FREQUENCY, DEPTH);
            SDL_SetRenderDrawColor(ctx->renderer, value, value, value, 0xFF);
            SDL_RenderDrawPoint(ctx->renderer, x, y);
        }
    }
}

void generate_vectorfield(SDL_Context *ctx, State *state) {
    SDL_SetRenderTarget(ctx->renderer, ctx->vectorfield);
    SDL_SetRenderDrawColor(ctx->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(ctx->renderer);

    for (int y = 0; y < FIELDHEIGHT; y++) {
        for (int x = 0; x < FIELDWIDTH; x++) {
            renderarrow(ctx, x*SPACING, y*SPACING, state->field[y][x]);
        }
    }
}

void generate_flowfield(SDL_Context *ctx, State *state) {
    SDL_SetRenderTarget(ctx->renderer, ctx->flowfield);
    SDL_SetRenderDrawColor(ctx->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(ctx->renderer);
    srand(SDL_GetTicks());
    int x, y;
    for (int i = 0; i < 10000; i++) {
        x = (rand() / (float) RAND_MAX) * FULLWIDTH;
        y = (rand() / (float) RAND_MAX) * FULLWIDTH;
        rendercurve(ctx, state, x, y);
        update(ctx, state);
    }
}

void cleanup_state(State *state) {
    free(state);
}

void cleanup(SDL_Context *ctx, State *state) {
    cleanup_context(ctx);
    cleanup_state(state);
    SDL_Quit();
    exit(0);
}

int main() {
    State *state = malloc(sizeof(State));
    for (int y = 0; y < FIELDHEIGHT; y++) {
        for (int x = 0; x < FIELDWIDTH; x++) {
            float angle = perlin(x, y, FREQUENCY, DEPTH) * 2 * PI;
            state->field[y][x] = angle;
        }
    }
    state->mousepos = malloc(sizeof(SDL_Point));

    SDL_Context *ctx = init_context();
    state->activetexture = ctx->flowfield;
    generate_flowfield(ctx, state);
    // generate_vectorfield(ctx, state);
    // generate_perlin(ctx, state);

    // renderfield(ctx, state);
    while (1) {
        if (!needsupdate(ctx->lastupdate)) {
            SDL_Delay(1);
            continue;
        }
        handleevents(ctx, state);
        update(ctx, state);
    }
}

