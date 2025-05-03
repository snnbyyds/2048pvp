#include <board.h>
#include <game.h>
#include <ui.h>

int turn = 0;

static int check_2048() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int v = board[i][j];
            if (v == 2048)
                return 1;
            if (v == -2048)
                return -1;
        }
    }
    return 0;
}

static int check_with_largest_block() {
    int mx = INT_MIN;
    int ret = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int v = board[i][j];
            if (!v)
                continue;
            if (abs(v) > mx) {
                mx = v;
                ret = v > 0 ? 1 : -1;
            }
        }
    }
    return ret;
}

int rungame() {
    if (!ui_init()) {
        fprintf(stderr, "UI Init Failed! Quitting...\n");
        return EXIT_FAILURE;
    }
    int status = 0;
    ui_render();
    while (true) {
        ui_code_t cmd = ui_handle_event();
        if (cmd == UI_QUIT)
            break;
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
        spawn(turn == 0 ? 1 : -1);
        ui_render();
        status = check_2048();
        if (status)
            break;
        if (!movable())
            status = check_with_largest_block();
        if (status)
            break;
        turn ^= 1;
    }
    ui_cleanup();
    return EXIT_SUCCESS;
}
