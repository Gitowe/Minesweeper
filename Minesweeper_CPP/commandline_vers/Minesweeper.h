#ifndef MINESWEEPER
#define MINESWEEPER
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

// maximum size of a game grid (25x25)
#define MAXSIDE 25

using namespace std;

class Sweeper {
public:
    // constructor
    Sweeper();

	// functions that set the side and mines
    void setSide(int s);
    void setMines(int m);
    int getSide();

	// function that runs the game for the user
	void playMinesweeper();

    // functions that handles the user's input
    int numInput();
    char charInput();


private:
    // checks the spot (specific locaiton on double array) to make sure its valid
    bool checkSpot(int row, int column);

    // functions that keep the mines accounted for
    bool checkMine(int row, int column, char anyBoard[][MAXSIDE]);
    int countMines(int row, int column, char realBoard[][MAXSIDE]);
    void placeMines(int mines[][2], char realBoard[][MAXSIDE], int firstRow, int firstCol);

    // functions that create/show the board
    void displayBoard(char shownBoard[][MAXSIDE]);
    void setBoard(char realBoard[][MAXSIDE], char shownBoard[][MAXSIDE]);

    // functions that calculate the minesweeper game
    bool calculateMove(int row, int column, int *movesLeft,
                         char shownBoard[][MAXSIDE], char realBoard[][MAXSIDE],
                         int mines[][2]);

	private:
    // global variables
    int side;
    int mines;
};

void gameIntro();

#endif