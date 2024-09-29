#ifndef MINESWEEPER_H
#define MINESWEEPER_H

// This is arguably an unnecessary header file, but it's here for the sake of consistency.
// (And because the C++ version used one.)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define MAXSIDE 25
#define MAXMINES 99

typedef struct {
    int side;
    int mines;
} Sweeper;

void playMinesweeper(Sweeper *game);
void gameIntro();
#endif