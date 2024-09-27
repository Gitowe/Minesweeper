#include "Minesweeper.h"

void difficulty(Sweeper *game) {
    int level;
    int loop = 1;

    printf("\n\nCurrent difficulty levels\n");
    printf("1 - Beginner (9x9 grid, 10 mines)\n");
    printf("2 - Intermediate (16x16 grid, 40 mines)\n");
    printf("3 - Advanced (23x23 grid, 99 mines)\n");

    printf("\nSelect a difficulty level: ");

    while (loop) {
        if (scanf("%d*[^\n]", &level)) {
            if (level == 1) {
                game->side = 9;
                game->mines = 10;
                loop = 0;
            } else if (level == 2) {
                game->side = 16;
                game->mines = 40;
                loop = 0;
            } else if (level == 3) {
                game->side = 23;
                game->mines = 99;
                loop = 0;
            } else {
                printf("Invalid choice. Please select 1, 2, or 3: ");
            }
        } else {
            while (getchar() != '\n');
            printf("Please enter a valid number: ");
        }
    }
    printf("\n");
}

// Function to set a custom difficulty level
void custom(Sweeper *game) {
    int loop = 1;

    printf("\n\nCustom difficulty selected!\n");
    printf("\nEnter the amount of sides (maximum is 25, minimum is 1): ");

    while (loop) {
        int input;
        if (scanf("%d*[^\n]", &input)) {
            if (input > 0 && input <= 25) {
                game->side = input;
                loop = 0;
            } else {
                printf("Invalid amount. Please enter a value between 1 and 25: ");
            }
        } else {
            while (getchar() != '\n');
            printf("Please enter a valid number: ");
        }
    }

    printf("\nEnter the amount of mines (maximum is 99, minimum is 1): ");

    loop = 1;
    while (loop) {
        int input;
        if (scanf("%d*[^\n]", &input)) {
            if (input > 0 && input < pow(game->side, 2)) {
                game->mines = input;
                loop = 0;
            } else {
                printf("Amount entered was too low, too high, or had more mines than spaces available in grid\n");
                printf("Please try again: ");
            }
        } else {
            while (getchar() != '\n');
            printf("Please enter a valid number: ");
        }
    }
    printf("\n");
}

void gameIntro() {
    Sweeper game;
    bool loop = true;
    int input;

    printf("\n\nWelcome to my Minesweeper game!\n");

    printf("\nCurrent game types\n");
    printf("1 - Premade\n");
    printf("2 - Custom\n");
    
    printf("\nSelect a game type: ");

    while (loop) {
        if (scanf("%d*[^\n]", &input)) {
            if (input == 1) {
                difficulty(&game);
                loop = false;
            } else if (input == 2) {
                custom(&game);
                loop = false;
            } else {
                printf("Invalid choice. Please select 1 or 2: ");
            }
        } else {
            while (getchar() != '\n');
            printf("Please enter a valid number: ");
        }
    }
    playMinesweeper(&game);
    loop = false;
}

int main() {
    
    gameIntro();

    return 0;
}