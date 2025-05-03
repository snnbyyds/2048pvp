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
