#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#define IMAGEWIDTH  1920
#define IMAGEHEIGHT 1080
#define FULLWIDTH   (IMAGEWIDTH * 2)
#define FULLHEIGHT  (IMAGEHEIGHT * 2)
#define SPACING     10
#define FIELDWIDTH  (FULLWIDTH / SPACING)
#define FIELDHEIGHT (FULLWIDTH / SPACING)

#define PI  3.14

typedef struct {
    float field[FIELDHEIGHT][FIELDWIDTH];
    SDL_Point *mousepos;
    SDL_Texture *activetexture;
} State;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *flowfield;
    SDL_Texture *perlin;
    SDL_Texture *vectorfield;
    Uint32 lastupdate;
    SDL_Rect *dstrect;
    SDL_Rect *srcrect;
} SDL_Context;

SDL_Context *init_context();
void cleanup_context(SDL_Context *ctx);
void update(SDL_Context *ctx, State *state);
int needsupdate(Uint32 lastupdate);
void rendercurve(SDL_Context *ctx, State *state, int x, int y);
void renderarrow(SDL_Context *ctx, int x, int y, float theta);
void save_flowfield(SDL_Context *ctx);

void handleevents(SDL_Context *ctx, State *state);

float perlin(float x, float y, float freq, int depth);

void cleanup(SDL_Context *ctx, State *state);
