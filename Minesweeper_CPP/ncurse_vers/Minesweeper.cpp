#include "Minesweeper.h"
#include <ncurses/ncurses.h>
using namespace std;

// Global variables for offset positioning
int rowOffsets[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int colOffsets[] = {0, 1, 1, 1, 0, -1, -1, -1};

// Global variables to store player position
int playerRow = 0, playerCol = 0;

Sweeper::Sweeper() {}

// Functions that set the side and mines
void Sweeper::setSide(int s) {
    side = s;
}

void Sweeper::setMines(int m) {
    mines = m;
}

int Sweeper::getSide() {
    return side;
}

// Checks if location on board is valid (not outside board or not treated as gridpoint)
bool Sweeper::spotCheck(int row, int col) {
    return ((row < side) && (col < side) && (row >= 0) && (col >= 0));
}

// Creates the board
void Sweeper::boardSet(char realBoard[][MAXSIDE], char shownBoard[][MAXSIDE]) {
    srand(time(NULL));

    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            shownBoard[i][j] = realBoard[i][j] = '-';
        }
    }

    playerRow = 0;
    playerCol = 0;

    return;
}

// Returns true if mine is present
bool Sweeper::mineCheck(int row, int col, char board[][MAXSIDE]) {
    if (board[row][col] == '*') {
        return (true);
    } else {
        return (false);
    }
}

// Places mines around the board
void Sweeper::mineSet(int mineStorage[][2], int firstRow, int firstCol, char realBoard[][MAXSIDE]) {
    bool checkOpenSpot[MAXSIDE * MAXSIDE];
    memset(checkOpenSpot, false, sizeof(checkOpenSpot));

    for (int i = 0; i < mines;) {
        int randNum1 = rand() % side;
        int randNum2 = rand() % side;

        int x = randNum1;
        int y = randNum2;

        if ((x == firstRow && y == firstCol) || checkOpenSpot[randNum1 * randNum2]) {
            continue;
        }

        if (checkOpenSpot[randNum1 * randNum2] == false) {
            mineStorage[i][0] = x;
            mineStorage[i][1] = y;

            realBoard[mineStorage[i][0]][mineStorage[i][1]] = '*';
            checkOpenSpot[randNum1 * randNum2] = true;
            i++;
        }
    }

    return;
}

// Displays the board with colors and highlighting
void Sweeper::display(char shownBoard[][MAXSIDE]) {
    clear();
    mvprintw(0, 0, "   ");

    if (side <= 9) {
        for (int i = 0; i < side; i++) {
            printw("%d ", i + 1);
        }
        printw("\n");

        for (int i = 0; i < side; i++) {
            printw("%d  ", i + 1);

            for (int j = 0; j < side; j++) {
                char ch = shownBoard[i][j];
                if (i == playerRow && j == playerCol) {
                    attron(COLOR_PAIR(3)); // Highlight player position
                } else {
                    switch (ch) {
                        case '-':
                            attron(COLOR_PAIR(1)); // Unrevealed tile
                            break;
                        case '@':
                            attron(COLOR_PAIR(2)); // Marked tile
                            break;
                        default:
                            if (ch >= '1' && ch <= '8') {
                                attron(COLOR_PAIR(ch - '0' + 4)); // Number color
                            } else {
                                attron(COLOR_PAIR(4)); // Revealed tile
                            }
                            break;
                    }
                }
                printw("%c ", ch);
                attroff(COLOR_PAIR(ch - '0' + 4)); // Remove number color
                attroff(COLOR_PAIR(1)); // Remove unrevealed tile color
                attroff(COLOR_PAIR(2)); // Remove marked tile color
                attroff(COLOR_PAIR(3)); // Remove player position highlight
            }
            printw("\n");
        }
    } else {
        for (int i = 0; i < side; i++) {
            if (i <= 8) {
                printw("%d  ", i + 1);
            } else {
                printw("%d ", i + 1);
            }
        }

        printw("\n");

        for (int i = 0; i < side; i++) {
            if (i <= 8) {
                printw("%d  ", i + 1);
            } else {
                printw("%d ", i + 1);
            }

            for (int j = 0; j < side; j++) {
                char ch = shownBoard[i][j];
                if (i == playerRow && j == playerCol) {
                    attron(COLOR_PAIR(3)); // Highlight player position
                } else {
                    switch (ch) {
                        case '-':
                            attron(COLOR_PAIR(1)); // Unrevealed tile
                            break;
                        case '@':
                            attron(COLOR_PAIR(2)); // Marked tile
                            break;
                        default:
                            if (ch >= '1' && ch <= '8') {
                                attron(COLOR_PAIR(ch - '0' + 4)); // Number color
                            } else {
                                attron(COLOR_PAIR(4)); // Revealed tile
                            }
                            break;
                    }
                }
                printw("%c  ", ch);
                attroff(COLOR_PAIR(ch - '0' + 4)); // Remove number color
                attroff(COLOR_PAIR(1)); // Remove unrevealed tile color
                attroff(COLOR_PAIR(2)); // Remove marked tile color
                attroff(COLOR_PAIR(3)); // Remove player position highlight
            }
            printw("\n");
        }
    }
    refresh();
}

// Counts mines to ensure they are properly implemented
int Sweeper::mineCounter(int row, int col, char realBoard[][MAXSIDE]) {
    int count = 0;

    for (int i = 0; i < 8; ++i) {
        int newRow = row + rowOffsets[i];
        int newCol = col + colOffsets[i];

        if (spotCheck(newRow, newCol) && mineCheck(newRow, newCol, realBoard)) {
            count++;
        }
    }

    return count;
}

// Checks if player has clicked on mine or not
bool Sweeper::calcMinesweeper(int row, int col, int *movesLeft, char shownBoard[][MAXSIDE], char realBoard[][MAXSIDE], int mineStorage[][2]) {
    if (shownBoard[row][col] != '-') {
        return false;
    }

    if (realBoard[row][col] == '*') {
        shownBoard[row][col] = '*';

        for (int i = 0; i < mines; i++) {
            shownBoard[mineStorage[i][0]][mineStorage[i][1]] = '*';
        }

        display(shownBoard);

        printw("\nYou lost!\n");
        refresh();
        return true;
    } else {
        int count = mineCounter(row, col, realBoard);
        (*movesLeft)--;

        shownBoard[row][col] = count + '0';

        if (!count) {
            for (int i = 0; i < 8; ++i) {
                int newRow = row + rowOffsets[i];
                int newCol = col + colOffsets[i];

                if (spotCheck(newRow, newCol) && !mineCheck(newRow, newCol, realBoard)) {
                    calcMinesweeper(newRow, newCol, movesLeft, shownBoard, realBoard, mineStorage);
                }
            }
        }
        return false;
    }
}

// Player's perspective of the game
void Sweeper::playMinesweeper() {
    initscr();
    start_color();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    // Initialize color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // Default color for unrevealed tiles
    init_pair(2, COLOR_RED, COLOR_BLACK);    // Color for marked tiles
    init_pair(3, COLOR_BLACK, COLOR_GREEN);  // Highlight color for player's position
    init_pair(4, COLOR_CYAN, COLOR_BLACK);   // Color for revealed tiles
    init_pair(5, COLOR_BLUE, COLOR_BLACK);   // Color for number 1
    init_pair(6, COLOR_GREEN, COLOR_BLACK);  // Color for number 2
    init_pair(7, COLOR_YELLOW, COLOR_BLACK); // Color for number 3
    init_pair(8, COLOR_MAGENTA, COLOR_BLACK); // Color for number 4
    init_pair(9, COLOR_RED, COLOR_BLACK);    // Color for numbers 5 through 8

    bool gameOver = false;
    bool quiteGame = false;
    char realBoard[MAXSIDE][MAXSIDE], shownBoard[MAXSIDE][MAXSIDE];
    int movesLeft = side * side - mines;
    int mineStorage[mines][2];

    boardSet(realBoard, shownBoard);

    int currentMoveIndex = 0;

    while (!gameOver) {
        display(shownBoard);

        printw ("\nMove your position with the arrow keys.");
        printw ("\nPress 'Space' to mark a cell. Press 'Enter' to open a cell.");
        printw ("\nPress 'q' to quit the game.\n");

        int ch = getch();

        switch (ch) {
        case KEY_UP:
            if (playerRow > 0) playerRow--;
            break;
        case KEY_DOWN:
            if (playerRow < side - 1) playerRow++;
            break;
        case KEY_LEFT:
            if (playerCol > 0) playerCol--;
            break;
        case KEY_RIGHT:
            if (playerCol < side - 1) playerCol++;
            break;
        case ' ':
            if (shownBoard[playerRow][playerCol] == '-') {
                shownBoard[playerRow][playerCol] = '@';
            } else if (shownBoard[playerRow][playerCol] == '@') {
                shownBoard[playerRow][playerCol] = '-';
            }
            break;
        case '\n':
            if (shownBoard[playerRow][playerCol] != '-') {
                printw("The cell has already been opened or marked. Please try again.\n");
                continue;
            }

            if (currentMoveIndex == 0) {
                mineSet(mineStorage, playerRow, playerCol, realBoard);
            }

            currentMoveIndex++;
            gameOver = calcMinesweeper(playerRow, playerCol, &movesLeft, shownBoard, realBoard, mineStorage);

            // Check if player won
            if (!gameOver && movesLeft == 0) {
                display(shownBoard);
                printw("\nYou won!\n");
                refresh();
                gameOver = true;
            }
            break;
        case 'q':
        case 'Q':
            printw("Quitting the game...\n");
            quiteGame = true;
            refresh();
            endwin();
            return;
        default:
            printw("Invalid action. Please try again.\n");
            refresh();
            break;
        }
    }
    while (!quiteGame) {
        printw("Press any key to continue or 'q' to quit: ");
        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            quiteGame = true;
        } else {
            playMinesweeper();
        }
    }

    endwin();
}