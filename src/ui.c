#include <ui.h>
#include <board.h>

// FIXME: Stop hardcoding font
static const char *font = "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf";

static SDL_Window *gWindow = NULL;
static SDL_Renderer *gRenderer = NULL;
static TTF_Font *gFont = NULL;

bool ui_init() {
    if (SDL_Init(SDL_INIT_VIDEO))
        return false;
    if (TTF_Init())
        return false;
    gWindow = SDL_CreateWindow(
        "2048 PvP", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_SIZE, WINDOW_SIZE,
        SDL_WINDOW_SHOWN
    );
    if (!gWindow)
        return false;
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer)
        return false;
    return (gFont = TTF_OpenFont(font, 24));
}

void ui_render() {
    SDL_SetRenderDrawColor(gRenderer, 240, 240, 240, 255); // rgba
    SDL_RenderClear(gRenderer);
    int cell = WINDOW_SIZE / N;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int v = board[i][j];
            SDL_Rect r = { .x = j * cell + 1, .y = i * cell + 1, .w = cell - 2, .h = cell - 2 };
            if (v > 0)
                SDL_SetRenderDrawColor(gRenderer, 200, 230, 200, 255);
            else if (v < 0)
                SDL_SetRenderDrawColor(gRenderer, 230, 200, 200, 255);
            else
                SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);
            SDL_RenderFillRect(gRenderer, &r);
            if (v) {
                char buf[18];
                snprintf(buf, 16, "%d", abs(v));
                SDL_Color color = {0, 0, 0, 255};
                SDL_Surface *surf = TTF_RenderText_Solid(gFont, buf, color);
                SDL_Texture *tx = SDL_CreateTextureFromSurface(gRenderer, surf);
                int w, h;
                SDL_QueryTexture(tx, NULL, NULL, &w, &h);
                SDL_Rect tr = {
                    .x = j * cell + ((cell - w) >> 1),
                    .y = i * cell + ((cell - h) >> 1),
                    .w = w, .h = h
                };
                SDL_RenderCopy(gRenderer, tx, NULL, &tr);
                SDL_DestroyTexture(tx);
                SDL_FreeSurface(surf);
            }
        }
    }
    SDL_RenderPresent(gRenderer);
}

ui_code_t ui_handle_event() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            return UI_QUIT;
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_w: case SDLK_UP:    return UI_UP;
                case SDLK_s: case SDLK_DOWN:  return UI_DOWN;
                case SDLK_a: case SDLK_LEFT:  return UI_LEFT;
                case SDLK_d: case SDLK_RIGHT: return UI_RIGHT;
                case SDLK_ESCAPE:             return UI_QUIT;
            }
        }
    }
    return UI_NONE;
}

void ui_delay(uint32_t ms) {
    SDL_Delay(ms);
}

void ui_cleanup() {
    if (gFont)
        TTF_CloseFont(gFont);
    if (gRenderer)
        SDL_DestroyRenderer(gRenderer);
    if (gWindow)
        SDL_DestroyWindow(gWindow);
    TTF_Quit();
    SDL_Quit();
}
