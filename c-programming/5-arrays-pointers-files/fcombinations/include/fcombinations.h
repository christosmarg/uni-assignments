#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "arrhandler.h"
#include "ccolors.h"

#define COMBSN 6

int get_n(FILE *);

void x_pair(int *, int *, FILE *);
void y_pair(int *, int *, FILE *);

void print_combs(int *, int, int, int, int, int);
void combinations(int *, int *, int *, int, int, int, int *, int *, int *, int, int, int, int, int);

bool even_calc(int *, int, int);
bool sum_comb_calc(int *, int, int);

void frequency(int *, int *, int *, int);
long int combinations_count(int);
long double factorial(int);
void print_other(int, int, int, int, int *, int *);

#endif
