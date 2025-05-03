#ifndef _UI_H_
#define _UI_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "common.h"

#define WINDOW_SIZE 640

typedef enum {
    UI_NONE  = 0,
    UI_UP    = 1,
    UI_DOWN  = 2,
    UI_LEFT  = 3,
    UI_RIGHT = 4,
    UI_QUIT  = -1
} ui_code_t;

bool ui_init();
void ui_render();
ui_code_t ui_handle_event();
void ui_delay(uint32_t ms);
void ui_cleanup();

#endif
