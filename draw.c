#include "flowfields.h"

#define FPS 60

void renderarrow(SDL_Context *ctx, int x, int y, float theta);

SDL_Context *init_context() {
    SDL_Context *ctx = malloc(sizeof(SDL_Context));

    SDL_Window *window = SDL_CreateWindow(
        "Flow Fields", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        IMAGEWIDTH, IMAGEHEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Texture *flowfield = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET, IMAGEWIDTH, IMAGEHEIGHT);
    SDL_Texture *perlin = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET, IMAGEWIDTH, IMAGEHEIGHT);
    SDL_Texture *vectorfield = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET, IMAGEWIDTH, IMAGEHEIGHT);

    Uint32 lastupdate = SDL_GetTicks();

    SDL_Rect *dstrect = malloc(sizeof(SDL_Rect));
    dstrect->x = 0;
    dstrect->y = 0;
    dstrect->w = IMAGEWIDTH;
    dstrect->h = IMAGEHEIGHT;

    SDL_Rect *srcrect = malloc(sizeof(SDL_Rect));
    srcrect->x = IMAGEWIDTH / 2;
    srcrect->y = IMAGEHEIGHT / 2;
    srcrect->w = IMAGEWIDTH;
    srcrect->h = IMAGEHEIGHT;

    ctx->window = window;
    ctx->renderer = renderer;
    ctx->flowfield = flowfield;
    ctx->perlin = perlin;
    ctx->vectorfield = vectorfield;
    ctx->lastupdate = lastupdate;
    ctx->dstrect = dstrect;
    ctx->srcrect = dstrect;

    return ctx;
}

void update(SDL_Context *ctx, State *state) {
    SDL_SetRenderTarget(ctx->renderer, NULL);
    SDL_RenderCopy(ctx->renderer, state->activetexture, NULL, NULL);
    SDL_RenderPresent(ctx->renderer);
    ctx->lastupdate = SDL_GetTicks();
}

void renderarrow(SDL_Context *ctx, int x, int y, float theta) {
    SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int x2 = x + 0.5 * SPACING * cos(theta);
    int y2 = y - 0.5 * SPACING * sin(theta);
    SDL_RenderDrawLine(ctx->renderer, x, y, x2, y2);
    SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0x00, 0x00, 0x00);
    SDL_RenderDrawPoint(ctx->renderer, x2, y2);
}

void rendercurve(SDL_Context *ctx, State *state, int x, int y) {
    int num_steps = 100;
    int step_length = 20;
    int x2, y2;
    SDL_SetRenderTarget(ctx->renderer, ctx->flowfield);
    for (int i = 0; i < num_steps; i++) {
        SDL_SetRenderDrawColor(ctx->renderer, 0xA0, 0x00, 0x00, 0xFF);
        double angle = state->field[y / SPACING][x / SPACING];
        x2 = x + step_length * cos(angle);
        y2 = y - step_length * sin(angle);
        if (!(x2 > IMAGEWIDTH || y2 > IMAGEHEIGHT || x2 < 0 || y2 < 0))
            SDL_RenderDrawLine(ctx->renderer, x, y, x2, y2);
        x = x2;
        y = y2;
    }
}

void save_flowfield(SDL_Context *ctx) {
    SDL_SetRenderTarget(ctx->renderer, ctx->flowfield);
    int width, height;
    SDL_QueryTexture(ctx->flowfield, NULL, NULL, &width, &height);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(ctx->renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, "output.png");
    SDL_FreeSurface(surface);
}

int needsupdate(Uint32 lastupdate) {
    return (SDL_GetTicks() - lastupdate) > (1000 / FPS);
}

void cleanup_context(SDL_Context *ctx) {
    SDL_DestroyWindow(ctx->window);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyTexture(ctx->flowfield);
    SDL_DestroyTexture(ctx->perlin);
    SDL_DestroyTexture(ctx->vectorfield);
    free(ctx->dstrect);
    free(ctx);
}
