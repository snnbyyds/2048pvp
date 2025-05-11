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
                mx = abs(v);
                ret = check_owner(v);
            }
        }
    }
    return ret;
}

static void display_result(player_t winner) {
    assert(winner != PLAYER_NONE);
    static const SDL_Color color = {13, 116, 216, 255};
    if (winner == PLAYER_A)
        ui_prompt("PLAYER A WINS!!", 4096, &color);
    else
        ui_prompt("PLAYER B WINS!!", 4096, &color);
}

static void display_player(player_t player) {
    static const SDL_Color color_a = {10, 134, 9, 255};
    static const SDL_Color color_b = {229, 40, 114, 255};
    if (player == PLAYER_A)
        ui_prompt("PLAYER A's turn", 512, &color_a);
    else
        ui_prompt("PLAYER B's turn", 512, &color_b);
}

static void welcome() {
    const SDL_Color color = {4, 241, 181, 255};
    ui_prompt("Welcome to 2048pvp", 2048, &color);
}

int rungame(bool demo) {
    static menu_choice_t menu_choice;
    player_t winner = PLAYER_NONE;
    player_t cur_player = PLAYER_A;
    bool pvc = false;
    if (!ui_init()) {
        fprintf(stderr, "UI Init Failed! Quitting...\n");
        return EXIT_FAILURE;
    }
label_menu:
    menu_choice = demo ? MENU_DEMO : ui_show_menu();
    assert(menu_choice != MENU_NONE);
    switch (menu_choice) {
        case MENU_PVP: pvc = demo = false; break;
        case MENU_PVC: pvc = true; demo = false; break;
        case MENU_DEMO: demo = true; pvc = false; break;
        case MENU_EXIT: ui_cleanup(); return EXIT_SUCCESS;
        case MENU_NONE: __builtin_unreachable(); break;
    }
    winner = PLAYER_NONE;
    cur_player = PLAYER_A;
    init_board();
    ui_render();
    welcome();
    for (int i = 0; i < 2; i++) {
        spawn(1);
        spawn(-1);
    }
    display_player(cur_player);
    while (true) {
        ui_code_t cmd = ui_handle_event();
        if (cmd == UI_QUIT) {
            ui_prompt("Exiting...", 512, NULL);
            ui_cleanup();
            return EXIT_SUCCESS;
        }
        if (cmd == UI_RETURN) {
            ui_prompt("Back to main menu.", 512, NULL);
            demo = false;
            goto label_menu;
        }
        if (demo || (pvc && cur_player == PLAYER_B)) {
            // override cmd for AI
            int rv = rand();
            if (pvc)
                ui_prompt("Bot is thinking...", ((double)(rv % 100) / 100.0 + 0.2) * 1024, NULL);
            int choice = rv % 4;
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
        spawn(cur_player == PLAYER_B ? PLAYER_A : PLAYER_B);
        ui_render();
        winner = check_2048();
        if (winner != PLAYER_NONE)
            break;
        if (!movable())
            winner = check_with_largest_block();
        if (winner != PLAYER_NONE)
            break;
        cur_player = cur_player == PLAYER_B ? PLAYER_A : PLAYER_B;
        display_player(cur_player);
    }
    display_result(winner);
    while (true) {
        ui_code_t cmd = ui_handle_event();
        if (cmd == UI_QUIT)
            break;
        if (cmd == UI_RETURN) {
            demo = false;
            goto label_menu;
        }
        ui_delay(8);
    }
    ui_cleanup();
    return EXIT_SUCCESS;
}
