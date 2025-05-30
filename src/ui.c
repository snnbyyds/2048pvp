/*
 * Copyright 2025 Nuo Shen, Nanjing University
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <board.h>
#include <ui.h>

static const char *background_music = "assets/background.mp3";
static const char *sound_effect = "assets/move.wav";
static const char *cursor = "assets/cursor.png";
static const char *font = "assets/Morning Routine.otf";
static const char *menu_text[MENU_ITEMS] = {"Start Game (PvP)",
                                            "Start Game (PvC)", "Demo", "Exit"};

static SDL_Window *gWindow = NULL;
static SDL_Renderer *gRenderer = NULL;
static SDL_Cursor *gCursor = NULL;
static Mix_Music *gMusic = NULL;
static Mix_Chunk *gSoundEffect = NULL;
static TTF_Font *gFont = NULL;
static SDL_Color gFontColor = {32, 32, 32, 255};

bool ui_init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        return false;
    if (TTF_Init() || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048))
        return false;
    gMusic = Mix_LoadMUS(background_music);
    gSoundEffect = Mix_LoadWAV(sound_effect);
    if (!gMusic || !gSoundEffect)
        return false;
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_VolumeChunk(gSoundEffect, MIX_MAX_VOLUME);
    gWindow = SDL_CreateWindow("2048 PvP", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE,
                               SDL_WINDOW_SHOWN);
    if (!gWindow)
        return false;
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer)
        return false;
    SDL_Surface *surf = IMG_Load(cursor);
    if (!surf)
        return false;
    gCursor = SDL_CreateColorCursor(surf, 2, 2);
    SDL_FreeSurface(surf);
    SDL_SetCursor(gCursor);
    Mix_PlayMusic(gMusic, -1);
    return (gFont = TTF_OpenFont(font, 48));
}

static void ui_draw_block(int v, int i, int j) {
    int cell = WINDOW_SIZE / N;
    SDL_Rect r = {
        .x = j * cell + 1, .y = i * cell + 1, .w = cell - 2, .h = cell - 2};
    if (v > 0)
        SDL_SetRenderDrawColor(gRenderer, 200, 230, 200, 255); // A
    else if (v < 0)
        SDL_SetRenderDrawColor(gRenderer, 230, 200, 200, 255); // B
    else
        SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);
    SDL_RenderFillRect(gRenderer, &r);
    if (v) {
        char buf[18];
        snprintf(buf, 16, "%d", abs(v));
        SDL_Surface *surf = TTF_RenderText_Solid(gFont, buf, gFontColor);
        SDL_Texture *tx = SDL_CreateTextureFromSurface(gRenderer, surf);
        int w, h;
        SDL_QueryTexture(tx, NULL, NULL, &w, &h);
        SDL_Rect tr = {.x = j * cell + ((cell - w) >> 1),
                       .y = i * cell + ((cell - h) >> 1),
                       .w = w,
                       .h = h};
        SDL_RenderCopy(gRenderer, tx, NULL, &tr);
        SDL_DestroyTexture(tx);
        SDL_FreeSurface(surf);
    }
}

static void ui_render_background() {
    {
        static SDL_Color grad_start = {224, 109, 73, 255};
        static SDL_Color grad_end = {33, 243, 205, 255};
        uint32_t p = (SDL_GetTicks() / 12) % (WINDOW_SIZE * 2);
        float t =
            (float)(p < WINDOW_SIZE ? p : (WINDOW_SIZE * 2) - p) / WINDOW_SIZE;
        uint8_t r = grad_start.r * (1 - t) + grad_end.r * t;
        uint8_t g = grad_start.g * (1 - t) + grad_end.g * t;
        uint8_t b = grad_start.b * (1 - t) + grad_end.b * t;
        uint8_t a = 255;
        SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
        SDL_Rect rect = {0, 0, WINDOW_SIZE, WINDOW_SIZE};
        SDL_RenderFillRect(gRenderer, &rect);
    }
    {
        static int i = 0, j = 0, v = 2, s = 1;
        static uint32_t start = 0;
        if (SDL_GetTicks() - start > 512) {
            i = rand() % N;
            j = rand() % N;
            v = 1 << (rand() % 12);
            if (v < 2)
                v <<= 1;
            s = rand() & 1 ? -1 : 1;
            start = SDL_GetTicks();
        }
        ui_draw_block(v * s, i, j);
    }
}

menu_choice_t ui_show_menu() {
    static SDL_Rect item_rects[MENU_ITEMS];
    static menu_choice_t sel = MENU_PVP;
    int y0 = WINDOW_SIZE >> 2;
    for (int i = 0; i < MENU_ITEMS; i++) {
        int tw, th;
        TTF_SizeText(gFont, menu_text[i], &tw, &th);
        item_rects[i] = (SDL_Rect){.x = (WINDOW_SIZE - tw) >> 1,
                                   .y = y0 + i * (th + 48),
                                   .w = tw,
                                   .h = th};
    }
    while (true) {
        ui_render_background();
        for (int i = 0; i < MENU_ITEMS; i++) {
            SDL_Color color = (i == sel) ? (SDL_Color){255, 255, 0, 255}
                                         : (SDL_Color){200, 200, 200, 255};
            SDL_Surface *surf =
                TTF_RenderText_Solid(gFont, menu_text[i], color);
            SDL_Texture *tx = SDL_CreateTextureFromSurface(gRenderer, surf);
            int w, h;
            SDL_QueryTexture(tx, NULL, NULL, &w, &h);
            SDL_RenderCopy(gRenderer, tx, NULL, item_rects + i);
            SDL_DestroyTexture(tx);
            SDL_FreeSurface(surf);
        }
        SDL_RenderPresent(gRenderer);
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                return MENU_EXIT;
            if (e.type == SDL_KEYDOWN) {
                ui_play_sound();
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        sel = (sel + MENU_ITEMS - 1) % MENU_ITEMS;
                        break;
                    case SDLK_DOWN: sel = (sel + 1) % MENU_ITEMS; break;
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER: return sel;
                    case SDLK_ESCAPE: return MENU_EXIT;
                }
            } else if (e.type == SDL_MOUSEMOTION) {
                int mx = e.motion.x, my = e.motion.y;
                for (int i = 0; i < MENU_ITEMS; i++) {
                    if (i != sel && mx >= item_rects[i].x &&
                        mx < item_rects[i].x + item_rects[i].w &&
                        my >= item_rects[i].y &&
                        my < item_rects[i].y + item_rects[i].h) {
                        sel = i;
                        ui_play_sound();
                    }
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int mx = e.button.x, my = e.button.y;
                    for (int i = 0; i < MENU_ITEMS; i++) {
                        if (mx >= item_rects[i].x &&
                            mx < item_rects[i].x + item_rects[i].w &&
                            my >= item_rects[i].y &&
                            my < item_rects[i].y + item_rects[i].h) {
                            ui_play_sound();
                            return i;
                        }
                    }
                }
            }
        }
        SDL_Delay(64);
    }
    __builtin_unreachable();
    return MENU_NONE;
}

void ui_render() {
    SDL_SetRenderDrawColor(gRenderer, 240, 240, 240, 255);
    SDL_RenderClear(gRenderer);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int v = board[i][j];
            ui_draw_block(v, i, j);
        }
    }
    SDL_RenderPresent(gRenderer);
}

void ui_play_sound() { Mix_PlayChannel(-1, gSoundEffect, 0); }

void ui_prompt(const char *s, uint32_t delay_ms, const SDL_Color *fontcolor) {
    assert(s);
    ui_render();
    SDL_Surface *surf =
        TTF_RenderText_Solid(gFont, s, fontcolor ? *fontcolor : gFontColor);
    SDL_Texture *tx = SDL_CreateTextureFromSurface(gRenderer, surf);
    int w, h;
    SDL_QueryTexture(tx, NULL, NULL, &w, &h);
    SDL_Rect dst = {(WINDOW_SIZE - w) >> 1, (WINDOW_SIZE - h) >> 1, w, h};
    SDL_RenderCopy(gRenderer, tx, NULL, &dst);
    SDL_RenderPresent(gRenderer);
    ui_delay(delay_ms);
    SDL_DestroyTexture(tx);
    SDL_FreeSurface(surf);
    ui_render();
}

ui_code_t ui_handle_event() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            return UI_QUIT;
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_w:
                case SDLK_UP: return UI_UP;
                case SDLK_s:
                case SDLK_DOWN: return UI_DOWN;
                case SDLK_a:
                case SDLK_LEFT: return UI_LEFT;
                case SDLK_d:
                case SDLK_RIGHT: return UI_RIGHT;
                case SDLK_q: return UI_RETURN;
                case SDLK_ESCAPE: return UI_QUIT;
            }
        }
    }
    return UI_NONE;
}

void ui_delay(uint32_t ms) { SDL_Delay(ms); }

void ui_cleanup() {
    if (gCursor)
        SDL_FreeCursor(gCursor);
    if (gMusic)
        Mix_FreeMusic(gMusic);
    if (gSoundEffect)
        Mix_FreeChunk(gSoundEffect);
    if (gFont)
        TTF_CloseFont(gFont);
    if (gRenderer)
        SDL_DestroyRenderer(gRenderer);
    if (gWindow)
        SDL_DestroyWindow(gWindow);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}
