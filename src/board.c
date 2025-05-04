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

int board[N][N];

static bool randseed_init = false;

static inline void initseed() {
    srand((unsigned int)time(NULL));
    randseed_init = true;
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
static bool compress(int *line) {
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
}
