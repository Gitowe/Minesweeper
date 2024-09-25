#include "Minesweeper.h"
#include <ncurses/ncurses.h>
using namespace std;

void difficulty(Sweeper &game) {
    int level;
    bool loop = true;

    cout << endl 
         << "Current Difficulty Levels" << endl;
    cout << "Press 1 for Beginner difficulty " << endl
         << "9 x 9 grid and 10 total mines" << endl
         << endl;
    cout << "Press 2 for Intermediate difficulty " << endl
         << "16 x 16 grid and 40 total mines" << endl
         << endl;
    cout << "Press 3 for Advanced difficulty " << endl
         << "23 x 23 grid and 99 Mines" << endl
         << endl;

    while (loop == true) {
        if (cin >> level) {
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
                cout << "Please select one of the three settings: ";
            }
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number: ";
        }
    }
}

// Function to set a custom difficulty level
void custom(Sweeper &game) {
    bool loop = true;

    cout << endl << endl 
         << "Enter the Custom Difficulty Level!" << endl;
    cout << "Enter the amount of sides you want (max is 26, minimum is 1): ";

    while (loop == true) {
        int input;
        if (cin >> input) {
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

    cout << endl;
    cout << "Enter the amount of mines you want (max is 99, minimum is 1): ";

    loop = true;
    while (loop == true) {
        int input;
        if (cin >> input) {
            if (input > 0 && input < pow(game.getSide(), 2)) {
                game.setMines(input);
                loop = false;
            } else {
                cout << "Amount entered was too low, too high, or had more mines "
                        "than spaces available in grid"
                     << endl
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

int main() {
    Sweeper game;
    bool loop = true;

    cout << endl << endl << "Welcome to my Minesweeper game!" << endl;

    while (loop == true) {
        int input;
        bool loop2 = true;
        cout << endl << "Current game types" << endl
             << "1 - Premade" << endl
             << "2 - Custom" << endl;
        cout << "Select a game type: ";
        while (loop2 == true) {
            if (cin >> input) {
                if (input == 1) {
                    difficulty(game);
                    loop2 = false;
                } else if (input == 2) {
                    custom(game);
                    loop2 = false;
                } else {
                    cout << "Please select one of the two settings: ";
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

    return (0);
}