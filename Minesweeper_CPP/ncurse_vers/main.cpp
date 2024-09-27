#include "Minesweeper.h"
#include <ncurses/ncurses.h>
using namespace std;

void difficulty(Sweeper &game) {
    int level;
    bool loop = true;

    cout << endl << endl
         << "Current difficulty levels" << endl;
    cout << "1 - Beginner (9x9 grid, 10 mines)" << endl;
    cout << "2 - Intermediate (16x16 grid, 40 mines)" << endl;
    cout << "3 - Advanced (23x23 grid, 99 mines)" << endl;
    cout << endl 
         << "Select a difficulty level: ";

    while (loop == true) {
        if (cin >> level) {
            cin.ignore();
            if (level == 1) {
                game.setSide(9);
                game.setMines(10);
                loop = false;
            } else if (level == 2) {
                game.setSide(16);
                game.setMines(40);
                loop = false;
            } else if (level == 3) {
                game.setSide(23);
                game.setMines(99);
                loop = false;
            } else {
                cout << "Invalid choice. Please select 1, 2, or 3: ";
            }
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number: ";
        }
    }
    cout << endl;
}

// Function to set a custom difficulty level
void custom(Sweeper &game) {
    bool loop = true;

    cout << endl << endl 
         << "Custom difficulty selected!" << endl;
    cout << endl
         << "Enter the amount of sides (maximum is 25, minimum is 1): ";

    while (loop == true) {
        int input;
        if (cin >> input) {
            cin.ignore();
            if (input > 0 && input < 27) {
                game.setSide(input);
                loop = false;
            } else {
                cout << "Amount entered was too low or too high, please try again: ";
            }
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number: ";
        }
    }

    cout << endl << "Enter the amount of mines you want (maximum is 99, minimum is 1): ";

    loop = true;
    while (loop == true) {
        int input;
        if (cin >> input) {
            cin.ignore();
            if (input > 0 && input < pow(game.getSide(), 2)) {
                game.setMines(input);
                loop = false;
            } else {
                cout << "Amount entered was too low, too high, or had more mines "
                        "than spaces available in grid" << endl
                     << "Please try again: ";
            }
            game.setMines(input);
            loop = false;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number: ";
        }
    }
    cout << endl << endl;
}

void gameIntro() {
    Sweeper game;
    bool loop = true;
    int input;

    cout << endl << endl 
         << "Welcome to my Minesweeper game!" << endl;
    cout << endl 
         << "Current game types" << endl  
         << "1 - Premade" << endl
         << "2 - Custom" << endl
         << endl
         << "Select a game type: ";

    while (loop == true) {
        if (cin >> input) {
            cin.ignore();
            if (input == 1) {
                difficulty(game);
                loop = false;
            } else if (input == 2) {
                custom(game);
                loop = false;
            } else {
                cout << "Invalid choice. Please select 1 or 2: ";
            }
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number: ";
        }
    }
    game.playMinesweeper();
    loop = false;  // Exit the game after playing once
}

int main() {

    gameIntro();

    return 0;
}