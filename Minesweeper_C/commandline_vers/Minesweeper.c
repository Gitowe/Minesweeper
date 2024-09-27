#include "Minesweeper.h"

// For context, the board is graphed as such:
//
//      -col   +col
//  -row         V
//           *
//  +row >
//

// Global variables for offset positioning
int rowOffsets[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int colOffsets[] = {0, 1, 1, 1, 0, -1, -1, -1};

// Checks if location on board is valid (not outside board or not treated as gridpoint)
int checkSpot(int row, int col, int side) {
    return ((row < side) && (col < side) && (row >= 0) && (col >= 0));
}

// Checks if a mine is present at a location
bool checkMine(int row, int col, char board[][MAXSIDE]) {
    return (board[row][col] == '*');
}

// Sets the board to be displayed to the player
void setBoard(char realBoard[][MAXSIDE], char shownBoard[][MAXSIDE], int side) {
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            shownBoard[i][j] = realBoard[i][j] = '-';
        }
    }
}

// Places mines around the board
void placeMines(int mineStorage[][2], 
                char realBoard[][MAXSIDE], 
                int mines, int side, int firstRow, int firstCol) 
{
    srand(time(NULL));
    bool checkOpenSpot[MAXSIDE * MAXSIDE] = {0};

    for (int i = 0; i < mines;) {
        int randRow = rand() % side;
        int randCol = rand() % side;

        if ((randRow == firstRow && randCol == firstCol) || checkOpenSpot[randRow * randCol]) {
            continue;
        }

        if (checkOpenSpot[randRow * side + randCol] == false) {
            mineStorage[i][0] = randRow;
            mineStorage[i][1] = randCol;
            realBoard[randRow][randCol] = '*';
            checkOpenSpot[randRow * side + randCol] = true;
            i++;
        }
    }
}

// Displays board
void displayBoard(char shownBoard[][MAXSIDE], int side) {
    printf("   ");

    if (side <= 9) {
        for (int i = 0; i < side; i++) {printf("%d ", i + 1);}
        printf("\n");

        for (int i = 0; i < side; i++) {
            printf("%d  ", i + 1);
            for (int j = 0; j < side; j++) {printf("%c ", shownBoard[i][j]);}
            printf("\n");
        }

    } else {
        for (int i = 0; i < side; i++) {
            if (i <= 8) {printf("%d  ", i + 1);} 
            else {printf("%d ", i + 1); }
        }
        printf("\n");

        for (int i = 0; i < side; i++) {
            if (i <= 8) {printf("%d  ", i + 1);} 
            else {printf("%d ", i + 1);}

            for (int j = 0; j < side; j++) {printf("%c  ", shownBoard[i][j]);}
            printf("\n");
        }
    }
}

// Counts mines to ensure they are properly implemented
int countMines(int row, int col, char realBoard[][MAXSIDE], int side) {
    int count = 0;

    // Iterate through all eight possible neighbors
    for (int i = 0; i < 8; ++i) {
        int newRow = row + rowOffsets[i];
        int newCol = col + colOffsets[i];

        if (checkSpot(newRow, newCol, side) && checkMine(newRow, newCol, realBoard)) {
            count++;
        }
    }
    return count;
}

// Checks if player has clicked on a mine or not
bool calculateMove(int row, int col, int *movesLeft, 
                   char shownBoard[][MAXSIDE], char realBoard[][MAXSIDE], 
                   int mineStorage[][2], int mines, int side) 
{
    if (shownBoard[row][col] != '-') {
        return false;
    }

    if (realBoard[row][col] == '*') {
        shownBoard[row][col] = '*';

        for (int i = 0; i < mines; i++) {
            shownBoard[mineStorage[i][0]][mineStorage[i][1]] = '*';
        }

        printf("\nCurrent Status of Board:\n\n");
        displayBoard(shownBoard, side);

        printf("\nYou lost!\n");
        return true;

    } else {
        int mineCount = countMines(row, col, realBoard, side);
        (*movesLeft)--;

        shownBoard[row][col] = mineCount + '0';

        if (mineCount == 0) {
            for (int i = 0; i < 8; ++i) {
                int newRow = row + rowOffsets[i];
                int newCol = col + colOffsets[i];

                if (checkSpot(newRow, newCol, side) && !checkMine(newRow, newCol, realBoard)) {
                    calculateMove(newRow, newCol, movesLeft, shownBoard, realBoard, mineStorage, mines, side);
                }
            }
        }
        return false;
    }
}

// Gets numerical input from user
int numInput() {
    int input;
    bool validInput = false;

    while (!validInput) {
        if (!scanf("%d*[^\n]", &input)) {
            while (getchar() != '\n');
            printf("Invalid input number. Please try again: ");
        } else {
            return input;
            validInput = true;
        }
    }
    return 0;
}

// Gets character input from user
char charInput() {
    char inputBuffer[100] = {0};
    char input = ' ';

    if (fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL) {
        return input = toupper(inputBuffer[0]);
    } else { 
        printf("Invalid input. Please try again: "); 
    }
    return input;
}

// Player perspective of game
void playMinesweeper(Sweeper *game) {
    bool gameOver = false;
    bool quiteGame = false;
    int currentMoveIndex = 0;
    int movesLeft = game->side * game->side - game->mines, x, y;    
    int mineStorage[MAXSIDE * MAXSIDE][2];
    char realBoard[MAXSIDE][MAXSIDE], shownBoard[MAXSIDE][MAXSIDE];

    setBoard(realBoard, shownBoard, game->side);

    while (!gameOver) {
        printf("\nCurrent Status of Board:\n\n");
        displayBoard(shownBoard, game->side);

        // Ask for user input
        printf("\nEnter an action to do (type 'H' to see available actions): ");
        bool boardEdit = false;
        getchar();

        while (!boardEdit) {

            char action = charInput();
            if (action == ' ') {continue;}

            // Case for checking or marking a tile
            if (action == 'C' || action == 'M') {
                printf("\nEnter the column position (x - axis) -> ");
                y = numInput() - 1;
                
                printf("Enter the row position (y - axis) -> ");
                x = numInput() - 1;
                
                printf("\n");

                if (x < 0 || x >= game->side || y < 0 || y >= game->side) {
                    printf("Invalid move. Please try again.\n");
                    boardEdit = true;
                    continue;
                }
                boardEdit = true;
            }

            // Check a tile
            if (action == 'C') {
                if (shownBoard[x][y] != '-') {
                    printf("The cell has already been opened or marked. Please try again.\n");
                    continue;
                }

                if (currentMoveIndex == 0) {
                    placeMines(mineStorage, realBoard, game->mines, game->side, x, y);
                }

                currentMoveIndex++;
                gameOver = calculateMove(x, y, &movesLeft, shownBoard, realBoard, mineStorage, game->mines, game->side);

                // Check if player won
                if (!gameOver && movesLeft == 0) {
                    printf("\nYou won!\n");
                    gameOver = true;
                }

            // Mark/Unmark a tile
            } else if (action == 'M') {
                if (shownBoard[x][y] == '-') {
                    shownBoard[x][y] = '@';
                } else if (shownBoard[x][y] == '@') {
                    shownBoard[x][y] = '-';
                } else {
                    printf("Cannot mark this tile. Please try again.\n");
                    continue;
                }

            // Help menu
            } else if (action == 'H') {
                printf("\nAvailable actions:\n");
                printf("C - Check a tile\n");
                printf("M - Mark/Unmark a tile\n");
                printf("H - See available actions\n");
                printf("Q - Quit the game\n");

                printf("\nPlease select one of the available actions: ");

            // Quit the game
            } else if (action == 'Q') {
                printf("\nQuitting the game...\n");
                quiteGame = true;
                return;
            } else {
                printf("Invalid action. Please try again: ");
            }
        }
    }

    // User options at end of game
    printf("\nWould you like to play again? (Y/N): ");
    while (!quiteGame) {

        char action = charInput();
        if (action == ' ') {continue;}

        if (action == 'Y') {
            gameIntro();
            quiteGame = true;
        } else if (action == 'N') {
            printf("Thanks for playing!\n");
            quiteGame = true;
        } else {
            printf("Invalid input. Please try again.");
        }  
    }
}