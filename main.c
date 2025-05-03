#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define N 8
static int board[N][N];
static bool randseed_init = false;

static inline void initseed() {
    srand((unsigned int)time(NULL));
    randseed_init = true;
}

// Print the board to the console
void print_board() {
    system("clear"); // TODO: Make this compatible in more OS
    putchar('\n');
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int v = board[i][j];
            if (v == 0)
                printf(" . ");
            else if (v > 0)
                printf("W%d ", v);
            else
                printf("B%d ", -v);;
        }
        putchar('\n');
    }
    putchar('\n');
}

// Spawn randomly
void spawn(int sign) {
    if (!randseed_init)
        initseed();
    assert(sign == 1 || sign == -1);
    static int empty[N * N][2];
    int cnt = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) {
                empty[cnt][0] = i;
                empty[cnt][1] = j;
                cnt++;
            }
        }
    }
    if (cnt == 0)
        return;
    int idx = rand() % cnt;
    int r = empty[idx][0], c = empty[idx][1];
    board[r][c] = sign * 2;
}

// Compress numbers in a line, return true if there is a compress
bool compress(int *line) {
    bool ret = false;
    static int comp[N];
    int cnt = 0;
    for (int i = 0; i < N; i++)
        if (line[i])
            comp[cnt++] = line[i];
    for (int i = 0; i < cnt - 1; i++) {
        if (comp[i] == comp[i + 1]) {
            comp[i] += comp[i + 1];
            for (int j = i + 1; j < cnt - 1; j++)
                comp[j] = comp[j + 1];
            cnt--;
            ret = true;
        }
    }
    for (int i = 0; i < N; i++) {
        int v = (i < cnt ? comp[i] : 0);
        if (line[i] != v)
            ret = true;
        line[i] = v;
    }
    return ret;
}

static int line[N]; // shared temp space for move functions

bool move_left() {
    bool ret = false;
    for (int i = 0; i < N; i++)
        if (compress(board[i]))
            ret = true;
    return ret;
}

bool move_right() {
    bool ret = false;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            line[j] = board[i][N - 1 - j];
        if (compress(line))
            ret = true;
        for (int j = 0; j < N; j++)
            board[i][N - 1 - j] = line[j];
    }
    return ret;
}

bool move_up() {
    bool ret = false;
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++)
            line[i] = board[i][j];
        if (compress(line))
            ret = true;
        for (int i = 0; i < N; i++)
            board[i][j] = line[i];
    }
    return ret;
}

bool move_down() {
    bool ret = false;
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++)
            line[i] = board[N - 1 - i][j];
        if (compress(line))
            ret = true;
        for (int i = 0; i < N; i++)
            board[N - 1 - i][j] = line[i];
    }
    return ret;
}

bool movable() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int v = board[i][j];
            if (v == 0) // has space to generate new block
                return true;
            if (i + 1 < N && board[i + 1][j] == v)
                return true;
            if (j + 1 < N && board[i][j + 1] == v)
                return true;
        }
    }
    return false;
}

void init_board() {
    memset(board, 0, sizeof(board));
    for (int i = 0; i < 2; i++) {
        spawn(1);
        spawn(-1);
    }
    print_board();
}

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

int main() {
    init_board();
    rungame();
    return 0;
}
