#include "Minesweeper.h"
using namespace std;

// for context, the board is graphed as such:
//
//      -col   +col
//  -row
//           *
//  +row
//

// global variables for offset positioning
const int rowOffsets[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int colOffsets[] = {0, 1, 1, 1, 0, -1, -1, -1};

Sweeper::Sweeper() {}

// functions that set the side and mines
void Sweeper::setSide(int s) {side = s;}

void Sweeper::setMines(int m) {mines = m;}

int Sweeper::getSide() {return side;}

// Checks if a spot is valid
bool Sweeper::spotCheck(int row, int col) {
    return ((row < side) && (col < side) && (row >= 0) && (col >= 0));
}

// Sets the board to be displayed to the player
void Sweeper::boardSet(char realBoard[][MAXSIDE], char shownBoard[][MAXSIDE]) {
    srand(time(NULL));

    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            shownBoard[i][j] = realBoard[i][j] = '-';
        }
    }

    return;
}

// Checks if a mine is present at a given location
bool Sweeper::mineCheck(int row, int col, char board[][MAXSIDE]) {
    if (board[row][col] == '*') {
        return (true);
    } else {
        return (false);
    }
}

// Places mines randomly on the board
void Sweeper::mineSet(int mineStorage[][2], int firstRow, int firstCol, char realBoard[][MAXSIDE]) {
    bool checkOpenSpot[MAXSIDE * MAXSIDE];
    memset(checkOpenSpot, false, sizeof(checkOpenSpot));

    for (int i = 0; i < mines;) {
        int randNum1 = rand() % side;
        int randNum2 = rand() % side;

        if ((randNum1 == firstRow && randNum2 == firstCol) || checkOpenSpot[randNum1 * randNum2]) {
            continue;
        }

        if (checkOpenSpot[randNum1 * randNum2] == false) {
            mineStorage[i][0] = randNum1;
            mineStorage[i][1] = randNum2;

            realBoard[mineStorage[i][0]][mineStorage[i][1]] = '*';
            checkOpenSpot[randNum1 * randNum2] = true;
            i++;
        }
    }

    return;
}

// Displays the board
void Sweeper::display(char shownBoard[][MAXSIDE]) {
    cout << "   ";

    // Print column numbers
    if (side <= 9) {
        for (int i = 0; i < side; i++) {
            cout << i + 1 << " ";
        }
        cout << endl;

        for (int i = 0; i < side; i++) {
            cout << i + 1 << "  ";

            for (int j = 0; j < side; j++) {
                cout << shownBoard[i][j] << " ";
            }
            cout << endl;
        }

    // Print column numbers
    } else {
        for (int i = 0; i < side; i++) {
            if (i <= 8) {
                cout << i + 1 << "  ";
            } else {
                cout << i + 1 << " ";
            }
        }

        cout << endl;

        // Print row numbers
        for (int i = 0; i < side; i++) {
            if (i <= 8) {
                cout << i + 1 << "  ";
            } else {
                cout << i + 1 << " ";
            }

            for (int j = 0; j < side; j++) {
                cout << shownBoard[i][j] << "  ";
            }
            cout << endl;
        }
    }
    return;
}

// Counts the number of mines around a given cell
int Sweeper::mineCounter(int row, int col, char realBoard[][MAXSIDE]) {

    int count = 0;

    // Iterate through all 8 possible directions
    for (int i = 0; i < 8; ++i) {
        int newRow = row + rowOffsets[i];
        int newCol = col + colOffsets[i];

        if (spotCheck(newRow, newCol) && mineCheck(newRow, newCol, realBoard)) {
            count++;
        }
    }

    return count;
}

// Checks if the player has clicked a mine
bool Sweeper::calcMinesweeper(int row, int col, int *movesLeft, 
                              char shownBoard[][MAXSIDE], char realBoard[][MAXSIDE], 
                              int mineStorage[][2]) 
{
    if (shownBoard[row][col] != '-') {
        return (false);
    }

    if (realBoard[row][col] == '*') {
        shownBoard[row][col] = '*';

        for (int i = 0; i < mines; i++) {
            shownBoard[mineStorage[i][0]][mineStorage[i][1]] = '*';
        }

        cout << endl << "Current Status of Board : " << endl << endl;
        display(shownBoard);

        cout << endl << "You lost!" << endl;
        return (true);
    }
    else {
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
        return (false);
    }
}

int Sweeper::numInput() {
    int input;
    bool validInput = false;

    while (!validInput) {
        if (cin >> input) {
            cin.ignore();
            return input;
            validInput = true;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input number. Please try again: ";
        }
    }
    return 0;
}

char Sweeper::charInput() {
    string input;
    char action = ' ';

    if (getline(cin, input)) {
        if (input.length() == 1) {
            return action = toupper(input[0]);
        } else {
            cout << "Invalid input. Please try again: ";
        }
    } else {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please try again: ";
    }
    return action;
}

// Player perspective of game
void Sweeper::playMinesweeper() {
    bool gameOver = false;
    bool quiteGame = false;
    char realBoard[MAXSIDE][MAXSIDE], shownBoard[MAXSIDE][MAXSIDE];
    int movesLeft = side * side - mines, x, y;
    int mineStorage[mines][2];

    boardSet(realBoard, shownBoard);
    //mineSet(mineStorage, realBoard);

    int currentMoveIndex = 0;

    // Loop of the game running (from the player's perspective)
    while (!gameOver) {
        cout << endl << "Current Status of Board : " << endl << endl;
        display(shownBoard);

        cout << endl << "Enter an action to do (type 'H' to see available actions): ";
        bool boardEdit = false;

        // Loop to ensure outputted messages are more clearly visible
        while (!boardEdit) {

            char action = charInput();
            if (action == ' ') {continue;}
            
            // Case for checking or marking a tile
            if (action == 'C' || action == 'M') {
                cout << "\nEnter the column position (x - axis) -> ";
                y = numInput() - 1;

                cout << "Enter the row position (y - axis) -> ";
                x = numInput() - 1;

                cout << endl;

                if (x < 0 || x >= side || y < 0 || y >= side) {
                    cout << "Invalid move. Please try again." << endl;
                    continue;
                    boardEdit = true;
                }
                boardEdit = true; 
            }

            // Mark/Unmark a tile
            if (action == 'M') {
                if (shownBoard[x][y] == '-') {
                    shownBoard[x][y] = '@';
                } else if (shownBoard[x][y] == '@') {
                    shownBoard[x][y] = '-';
                } else {
                    cout << "Cannot mark this tile. Please try again." << endl;
                    continue;
                }
            }

            // Check a tile
            else if (action == 'C') {
                if (shownBoard[x][y] != '-') {
                    cout << "The cell has already been opened or marked. Please try again." << endl;
                    continue;
                }

                if (currentMoveIndex == 0) {
                    mineSet(mineStorage, x, y, realBoard);
                }

                currentMoveIndex++;
                gameOver = calcMinesweeper(x, y, &movesLeft, shownBoard, realBoard, mineStorage);

                // Check if player won
                if (!gameOver && movesLeft == 0) {
                    cout << endl << "You won!" << endl;
                    gameOver = true;
                } 

            // Help menu
            } else if (action == 'H') {
                cout << "Available actions: " << endl
                     << "C - Check a tile" << endl
                     << "M - Mark/Unmark a tile" << endl
                     << "H - See available actions" << endl
                     << "Q - Quit the game" << endl;

                cout << endl 
                     << "Please select one of the available actions: ";

            // Quit the game
            } else if (action == 'Q') {
                cout << "Quitting the game..." << endl;
                quiteGame = true;
                return;
            } else {
                cout << "Invalid action. Please try again: ";
            }
        }
    }

    cout << endl << "Would you like to play again? (Y/N): ";
    while(!quiteGame) {

        char action = charInput();
        if (action == ' ') {continue;}
        
        if (action == 'Y') {
            gameIntro();
            quiteGame = true;
        } else if (action == 'N') {
            cout << "Thanks for playing!" << endl;
            quiteGame = true;
        } else {
            cout << "Invalid action. Please try again. ";
        }
    }

    return;
}