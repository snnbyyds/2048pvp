#ifndef _BOARD_H_
#define _BOARD_H_

#include "common.h"

#define N 8 // Board size: 8 * 8

void init_board();

bool movable();
bool move_left();
bool move_right();
bool move_up();
bool move_down();

void spawn(int sign);

extern int board[N][N];

#endif
