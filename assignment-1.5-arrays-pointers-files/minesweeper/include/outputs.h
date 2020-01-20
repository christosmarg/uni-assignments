#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "minesweeper.h"

void game_won(struct _win_st*, char **, int, int);
void game_over(struct _win_st*, char **, int, int);

void print_board(struct _win_st*, char **, int, int);
void filewrite(char **, int, int, int, int, const char *);

#endif