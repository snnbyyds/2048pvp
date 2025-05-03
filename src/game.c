#include "board.h"
#include "game.h"

void rungame() {
    int turn = 0;
    char c;
    while (1) {
        printf("%s's turn (W/A/S/D): ", turn ? "Black" : "White");
        if (scanf(" %c", &c) != 1)
            continue;
        bool moved = false;
        switch (c) {
            case 'W': case 'w': moved = move_up(); break;
            case 'S': case 's': moved = move_down(); break;
            case 'D': case 'd': moved = move_right(); break;
            case 'A': case 'a': moved = move_left(); break;
        }
        if (!moved)
            continue;
        spawn(turn == 0 ? 1 : -1);
        print_board();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int v = board[i][j];
                if (v == 2048 || v == -2048) {
                    printf("%s wins!\n", board[i][j] > 0 ? "White" : "Black");
                    return;
                }
            }
        }
        if (!movable()) {
            int max_block = INT_MIN;
            int winner = -1;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    int v = abs(board[i][j]);
                    if (v > max_block) {
                        max_block = v;
                        winner = board[i][j] > 0 ? 0 : 1; // zero for white, maybe clarify with enum
                    }
                }
            }
            printf("%s wins by highest block!\n", winner == 0 ? "White" : "Black");
            return;
        }
        turn ^= 1;
    }
}
