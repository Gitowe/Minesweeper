#include "Minesweeper.h"
using namespace std;

// For context, the board is graphed as such:
//
//      -col   +col
//  -row
//           *
//  +row
//

// Global variables for offset positioning
const int rowOffsets[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int colOffsets[] = {0, 1, 1, 1, 0, -1, -1, -1};

Sweeper::Sweeper() {}

// Functions that set the side and mines
void Sweeper::setSide(int s) {side = s;}

void Sweeper::setMines(int m) {mines = m;}

int Sweeper::getSide() {return side;}

// Checks if location on board is valid (not outside board or not treated as gridpoint)
bool Sweeper::checkSpot(int row, int col) {
    return ((row < side) && (col < side) && (row >= 0) && (col >= 0));
}

// Checks if a mine is present at a given location
bool Sweeper::checkMine(int row, int col, char board[][MAXSIDE]) {
    return (board[row][col] == '*');
}

// Sets the board to be displayed to the player
void Sweeper::setBoard(char realBoard[][MAXSIDE], char shownBoard[][MAXSIDE]) {
    srand(time(NULL));

    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            shownBoard[i][j] = realBoard[i][j] = '-';
        }
    }
}

// Places mines randomly on the board
void Sweeper::placeMines(int mineStorage[][2], 
                         char realBoard[][MAXSIDE], 
                         int firstRow, int firstCol) {

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
}

// Displays the board
void Sweeper::displayBoard(char shownBoard[][MAXSIDE]) {
    cout << "   ";

    // Print column numbers
    if (side <= 9) {
        for (int i = 0; i < side; i++) {cout << i + 1 << " ";}
        cout << endl;

        for (int i = 0; i < side; i++) {
            cout << i + 1 << "  ";
            for (int j = 0; j < side; j++) {cout << shownBoard[i][j] << " ";}
            cout << endl;
        }

    // Print column numbers
    } else {
        for (int i = 0; i < side; i++) {
            if (i <= 8) {cout << i + 1 << "  ";}
            else {cout << i + 1 << " ";}
        }
        cout << endl;

        // Print row numbers
        for (int i = 0; i < side; i++) {
            if (i <= 8) {cout << i + 1 << "  ";} 
            else {cout << i + 1 << " ";}

            for (int j = 0; j < side; j++) {cout << shownBoard[i][j] << "  ";}
            cout << endl;
        }
    }
}

// Counts the number of mines around a given cell
int Sweeper::countMines(int row, int col, char realBoard[][MAXSIDE]) {

    int count = 0;

    // Iterate through all 8 possible directions
    for (int i = 0; i < 8; ++i) {
        int newRow = row + rowOffsets[i];
        int newCol = col + colOffsets[i];

        if (checkSpot(newRow, newCol) && checkMine(newRow, newCol, realBoard)) {
            count++;
        }
    }

    return count;
}

// Checks if the player has clicked a mine
bool Sweeper::calculateMove(int row, int col, int *movesLeft, 
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

        cout << endl << "Final Status of Board : " << endl << endl;
        displayBoard(shownBoard);

        cout << endl << "You lost!" << endl;
        return (true);
    }
    else {
        int count = countMines(row, col, realBoard);
        (*movesLeft)--;

        shownBoard[row][col] = count + '0';

        if (!count) {
            for (int i = 0; i < 8; ++i) {
                int newRow = row + rowOffsets[i];
                int newCol = col + colOffsets[i];

                if (checkSpot(newRow, newCol) && !checkMine(newRow, newCol, realBoard)) {
                    calculateMove(newRow, newCol, movesLeft, shownBoard, realBoard, mineStorage);
                }
            }
        }
        return (false);
    }
}

// Gets numerical input from user
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

// Gets character input from user
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
    int currentMoveIndex = 0;
    int movesLeft = side * side - mines, x, y;
    int mineStorage[MAXSIDE * MAXSIDE][2];
    char realBoard[MAXSIDE][MAXSIDE], shownBoard[MAXSIDE][MAXSIDE];

    setBoard(realBoard, shownBoard);
    cout << endl;

    while (!gameOver) {
        cout << endl << "Current Status of Board : " << endl << endl;
        displayBoard(shownBoard);

        // Ask for user input
        cout << endl << "Enter an action to do (type 'H' to see available actions): ";
        bool boardEdit = false;

        while (!boardEdit) {

            char action = charInput();
            if (action == ' ') {continue;}
            
            // Case for checking or marking a tile
            if (action == 'C' || action == 'M') {
                cout << endl 
                     << "Enter the column position (x - axis) -> ";
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

            // Check a tile
            if (action == 'C') {
                if (shownBoard[x][y] != '-') {
                    cout << "The cell has already been opened or marked. Please try again." << endl;
                    continue;
                }

                if (currentMoveIndex == 0) {
                    placeMines(mineStorage, realBoard, x, y);
                }

                currentMoveIndex++;
                gameOver = calculateMove(x, y, &movesLeft, shownBoard, realBoard, mineStorage);

                // Check if player won
                if (!gameOver && movesLeft == 0) {
                    cout << endl << "Final Status of Board : " << endl << endl;
                    for (int i = 0; i < mines; i++) {
                        shownBoard[mineStorage[i][0]][mineStorage[i][1]] = '@';
                    }
                    displayBoard(shownBoard);
                    cout << endl << "You won!" << endl;
                    gameOver = true;
                } 

            // Mark/Unmark a tile
            } else if (action == 'M') {
                if (shownBoard[x][y] == '-') {
                    shownBoard[x][y] = '@';
                } else if (shownBoard[x][y] == '@') {
                    shownBoard[x][y] = '-';
                } else {
                    cout << "Cannot mark this tile. Please try again." << endl;
                    continue;
                }

            // Help menu
            } else if (action == 'H') {
                cout << endl
                     << "Available actions: " << endl
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

    // User options at end of game
    cout << endl 
         << "Would you like to play again? (Y/N): ";
    while(!quiteGame) {

        char action = charInput();
        if (action == ' ') {continue;}
        
        if (action == 'Y') {
            gameIntro();
            quiteGame = true;
        } else if (action == 'N') {
            cout << endl << endl 
                 << "Thanks for playing!" << endl << endl;
            quiteGame = true;
        } else {
            cout << "Invalid action. Please try again: ";
        }
    }

    return;
}