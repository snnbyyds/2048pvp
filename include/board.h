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
