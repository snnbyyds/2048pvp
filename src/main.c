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

#include <SDL2/SDL_main.h>
#include <board.h>
#include <game.h>

int main(int argc, char **argv) {
    bool demo = false;
    for (int i = 0; i < argc; i++) {
        if (!strcmp("--demo", argv[i])) {
            demo = true;
            printf("Demo Mode is ON!\n");
            break;
        }
    }
    return rungame(demo);
}
