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
#include <game.h>
#include <ui.h>

static inline player_t check_owner(int block) {
    if (block > 0)
        return PLAYER_A;
    if (block < 0)
        return PLAYER_B;
    return PLAYER_NONE;
}

static player_t check_2048() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int v = board[i][j];
            if (abs(v) == 2048)
                return check_owner(v);
        }
    }
    return PLAYER_NONE;
}

static player_t check_with_largest_block() {
    int mx = INT_MIN;
    player_t ret = PLAYER_NONE;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int v = board[i][j];
            if (!v)
                continue;
            if (abs(v) > mx) {
                mx = v;
                ret = check_owner(v);
            }
        }
    }
    return ret;
}

static void display_result(player_t winner) {
    assert(winner != PLAYER_NONE);
    const SDL_Color color = {13, 116, 216, 255};
    if (winner == PLAYER_A)
        ui_prompt("PLAYER A WINS!!", 4096, &color);
    else
        ui_prompt("PLAYER B WINS!!", 4096, &color);
}

static void display_player(int turn) {
    if (turn == 0)
        ui_prompt("PLAYER A's turn", 512, NULL);
    else
        ui_prompt("PLAYER B's turn", 512, NULL);
}

static void welcome() {
    const SDL_Color color = {4, 241, 181, 255};
    ui_prompt("Welcome to 2048pvp", 2048, &color);
}

int rungame(bool demo) {
    if (!ui_init()) {
        fprintf(stderr, "UI Init Failed! Quitting...\n");
        return EXIT_FAILURE;
    }
    menu_choice_t menu_choice = demo ? MENU_DEMO : ui_show_menu();
    assert(menu_choice != MENU_NONE);
    switch (menu_choice) {
        case MENU_START: break;
        case MENU_DEMO: demo = true; break;
        case MENU_EXIT: ui_cleanup(); return EXIT_SUCCESS;
        case MENU_NONE: __builtin_unreachable(); break;
    }
    player_t winner = PLAYER_NONE;
    ui_render();
    welcome();
    int turn = 0;
    display_player(turn);
    while (true) {
        ui_code_t cmd = ui_handle_event();
        if (cmd == UI_QUIT) {
            ui_prompt("Exiting...", 512, NULL);
            ui_cleanup();
            return EXIT_SUCCESS;
        }
        if (demo) {
            // override cmd in demo mode
            int choice = rand() % 4;
            switch (choice) {
                case 0: cmd = UI_UP; break;
                case 1: cmd = UI_DOWN; break;
                case 2: cmd = UI_LEFT; break;
                case 3: cmd = UI_RIGHT; break;
            }
        }
        bool moved = false;
        switch (cmd) {
            case UI_UP: moved = move_up(); break;
            case UI_DOWN: moved = move_down(); break;
            case UI_LEFT: moved = move_left(); break;
            case UI_RIGHT: moved = move_right(); break;
            default: ui_delay(64); continue;
        }
        if (!moved)
            continue;
        ui_play_sound();
        spawn(turn == 0 ? PLAYER_A : PLAYER_B);
        ui_render();
        winner = check_2048();
        if (winner != PLAYER_NONE)
            break;
        if (!movable())
            winner = check_with_largest_block();
        if (winner)
            break;
        turn ^= 1;
        display_player(turn);
    }
    display_result(winner);
    while (ui_handle_event() != UI_QUIT);
    ui_cleanup();
    return EXIT_SUCCESS;
}
