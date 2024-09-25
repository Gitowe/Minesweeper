#ifndef MINESWEEPER
#define MINESWEEPER
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
// #include <bits/stdc++.h>

// maximum size of a game grid (25x25)
#define MAXSIDE 26

using namespace std;

class Sweeper {
  public:
    // constructor
    Sweeper();

    void setSide(int s);
    void setMines(int m);
    int getSide();

    // checks the spot (specific locaiton on double array) to make sure its
    // valid
    bool spotCheck(int row, int column);

    // functions that keep the mines accounted for
    bool mineCheck(int row, int column, char anyBoard[][MAXSIDE]);
    int mineCounter(int row, int column, char realBoard[][MAXSIDE]);
    void mineSet(int mines[][2], int firstRow, int firstCol, char realBoard[][MAXSIDE]);

    // functions that create/show the board
    void display(char shownBoard[][MAXSIDE]);
    void boardSet(char realBoard[][MAXSIDE], char shownBoard[][MAXSIDE]);

    // functions that run the game directly
    bool calcMinesweeper(int row, int column, int *movesLeft,
                         char shownBoard[][MAXSIDE], char realBoard[][MAXSIDE],
                         int mines[][2]);
    void playMinesweeper();

    // functions that set the difficulty
    void difficulty();
    void custom();

  private:
    // global variables
    int test;
    int side;
    int mines;

    void initColors();
};

#endif