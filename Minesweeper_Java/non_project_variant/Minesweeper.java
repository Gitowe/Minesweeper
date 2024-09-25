import java.util.Scanner;
import java.util.InputMismatchException;
import java.util.Random;

public class Minesweeper {
    private static final int MAXSIDE = 25;
    private static final int MAXMINES = 99;
    private static final int[] rowOffsets = {-1, -1, 0, 1, 1, 1, 0, -1};
    private static final int[] colOffsets = {0, 1, 1, 1, 0, -1, -1, -1};

    private static boolean isValid(int row, int col, int side) {
        return (row >= 0 && col >= 0 && row < side && col < side);
    }

    private static void setupBoard(char[][] realBoard, char[][] shownBoard, int side) {
        for (int i = 0; i < side; i++) {
            for (int j = 0; j < side; j++) {
                realBoard[i][j] = '-';
                shownBoard[i][j] = '-';
            }
        }
    }

    private static void placeMines(int[][] mineStorage, char[][] realBoard, int mines, int side, int firstRow, int firstCol) {
        Random random = new Random();
        boolean[] checkOpenSpot = new boolean[MAXSIDE * MAXSIDE];

        for (int i = 0; i < mines;) {
            int randRow = random.nextInt(side);
            int randCol = random.nextInt(side);

            if ((randRow == firstRow && randCol == firstCol) || checkOpenSpot[randRow * randCol]) {
                continue;
            }

            if (!checkOpenSpot[randRow * side + randCol]) {
                mineStorage[i][0] = randRow;
                mineStorage[i][1] = randCol;
                realBoard[randRow][randCol] = '*';
                checkOpenSpot[randRow * side + randCol] = true;
                i++;
            }
        }
    }

    private static void displayBoard(char[][] board, int side) {
        System.out.print("   ");
        for (int i = 1; i <= side; i++) {
            System.out.print(i + " ");
        }
        System.out.println();

        for (int i = 0; i < side; i++) {
            System.out.print((i + 1) + " ");
            if (i < 9) System.out.print(" ");
            for (int j = 0; j < side; j++) {
                System.out.print(board[i][j] + " ");
            }
            System.out.println();
        }
    }

    private static int countMines(int row, int col, char[][] realBoard, int side) {
        int count = 0;
        for (int i = 0; i < 8; i++) {
            int newRow = row + rowOffsets[i];
            int newCol = col + colOffsets[i];
            if (isValid(newRow, newCol, side) && realBoard[newRow][newCol] == '*') {
                count++;
            }
        }
        return count;
    }

    private static boolean calculateMove(int row, int col, int movesLeft, char[][] shownBoard, char[][] realBoard, int[][] mineStorage, int mines, int side) {
        if (realBoard[row][col] == '*') {
            shownBoard[row][col] = '*';
            for (int i = 0; i < mines; i++) {
                shownBoard[mineStorage[i][0]][mineStorage[i][1]] = '*';
            }
            System.out.println("\nCurrent Status of Board:\n\n");

            displayBoard(shownBoard, side);
            System.out.println("\nYou lost!");
            return true;
        } else {
            int mineCount = countMines(row, col, realBoard, side);
            shownBoard[row][col] = (char) (mineCount + '0');
            if (mineCount == 0) {
                for (int i = 0; i < 8; i++) {
                    int newRow = row + rowOffsets[i];
                    int newCol = col + colOffsets[i];
                    if (isValid(newRow, newCol, side) && realBoard[newRow][newCol] != '*') {
                        calculateMove(newRow, newCol, shownBoard, realBoard, mineStorage, mines, side, movesLeft);
                    }
                }
            }
            return false;
        }
    }

    private static int numInput(Scanner scanner) {
        while (true) {
            if (scanner.hasNextInt()) {
                int input = scanner.nextInt();
                return input;
            } else {
                System.out.print("Invalid input. Please try again: ");
                scanner.next();
            }
        }
    }

    private static char charInput(Scanner scanner) {
        while (true) {
            try {
                char input = scanner.next().toUpperCase().charAt(0);
                return input;
            } catch (InputMismatchException e) {
                System.out.print("Invalid input. Please try again: ");
                scanner.next();
            }
        }
    }

    private static void playMinesweeper(Sweeper game) {
        char[][] realBoard = new char[game.getSide()][game.getSide()];
        char[][] shownBoard = new char[game.getSide()][game.getSide()];
        int[][] mineStorage = new int[MAXMINES][2];
        int movesLeft = game.getSide() * game.getSide() - game.getMines(), x = 0, y = 0;
        int currentMoveIndex = 0;
        boolean gameOver = false;

        setupBoard(realBoard, shownBoard, game.getSide());

        Scanner scanner = new Scanner(System.in);
        while (!gameOver) {
            System.out.println("\nCurrent Status of Board:\n\n");
            displayBoard(shownBoard, game.getSide());

            System.out.print("\nEnter an action to do (type 'H' to see available actions): ");
            boolean boardEdit = false;

            while (!boardEdit) {
                try {
                    char action = charInput(scanner);
                    if (action == ' ') {continue;}

                    if (action == 'C' || action == 'M') {
                        System.out.print("Enter the column position (x - axis) -> ");
                        y = numInput(scanner) - 1;

                        System.out.print("Enter the row position (y - axis) -> ");
                        x = numInput(scanner) - 1;

                        if (x < 0 || x >= side || y < 0 || y >= side) {
                            System.out.print("Invalid position. Please try again: ");
                            continue;
                            boardEdit = true;
                        }
                        boardEdit = true;
                    }

                    switch (action) {
                        case 'C':
                            if (shownBoard[x][y] != '-') {
                                System.out.print("The cell has already been opened or marked. Please try again.\n");
                                break;
                            }

                            if (currentMoveIndex == 0) {
                                placeMines(mineStorage, realBoard, game.getMines(), game.getSide(), x, y);
                            }

                            currentMoveIndex++;
                            gameOver = calculateMove(x, y, movesLeft, shownBoard, realBoard, mineStorage, game.getMines(), game.getSide());
                            
                            movesLeft--;
                            if (!gameOver && movesLeft == 0) {
                                System.out.println("\nYou won!");
                                gameOver = true;
                            }
                            break;
                        case 'M':
                            if (shownBoard[x][y] == '-') {
                                shownBoard[x][y] = '@';
                            } else if (shownBoard[x][y] == '@') {
                                shownBoard[x][y] = '-';
                            }
                            break;
                        case 'H':
                            System.out.println("\nAvailable actions:");
                            System.out.println("C - Check a tile");
                            System.out.println("M - Mark/Unmark a tile");
                            System.out.println("H - See available actions");
                            System.out.println("Q - Quit the game");
                            
                            System.out.print("Please select one of the available actions: ");
                            break;
                        case 'Q':
                            System.out.println("\nQuitting the game...");
                            System.exit(0);
                            break;
                        default:
                            System.out.print("Invalid action. Please try again: ");
                    }
                } catch (InputMismatchException e) {
                    System.out.print("Invalid input. Please try again: ");
                    scanner.next();
                }
            }
        }
    }


    //////////////////////////
    // Game Startup Methods //
    //////////////////////////


    private static void custom(Sweeper game) {
        Scanner scanner = new Scanner(System.in);
        boolean validInput = false;

        System.out.println("\n\nHere are the options to select for a custom difficulty!\n");
        System.out.print("\n\nEnter the amount of sides (max is 25, minimum is 1): ");
        while (!validInput) {
            if(scanner.hasNextInt()){
                int side = scanner.nextInt();
                if (side > 0 && side <= 25) {
                    game.setSide(side);
                    validInput = true;
                } else {
                    System.out.print("Invalid amount. Please enter a value between 1 and 25: ");
                }
            } else {
                System.out.print("Please enter a valid number: ");
                scanner.next();
            }
        }

        validInput = false;
        System.out.print("Enter the amount of mines (max is 99, minimum is 1): ");
        while (!validInput) {
            if(scanner.hasNextInt()){
                int mines = scanner.nextInt();
                if (mines > 0 && mines <= 99) {
                    game.setMines(mines);
                    validInput = true;
                } else {
                    System.out.print("Invalid amount. Please enter a value between 1 and 99: ");
                }
            } else {
                System.out.print("Please enter a valid number: ");
                scanner.next();
            }
        }
    }

    private static void difficulty(Sweeper game) {
        Scanner scanner = new Scanner(System.in);
        boolean validInput = false;

        System.out.println("\n\nCurrent difficulty levels");
        System.out.println("1 - Beginner (9x9 grid, 10 mines)");
        System.out.println("2 - Intermediate (16x16 grid, 40 mines)");
        System.out.println("3 - Advanced (23x23 grid, 99 mines)");

        System.out.print("Select a difficulty level: ");

        while (!validInput) {
            if (scanner.hasNextInt()){
                int level = scanner.nextInt();
                switch (level) {
                    case 1:
                        game.setSide(9);
                        game.setMines(10);
                        validInput = true;
                        break;
                    case 2:
                        game.setSide(16);
                        game.setMines(40);
                        validInput = true;
                        break;
                    case 3:
                        game.setSide(23);
                        game.setMines(99);
                        validInput = true;
                        break;
                    default:
                        System.out.println("Invalid choice. Please select 1, 2, or 3.");
                }
            } else {
                System.out.print("Please enter a valid number: ");
                scanner.next();
            }
        }
    }

    private static void gameIntro() {
        Sweeper game = new Sweeper(9, 10);
        Scanner scanner = new Scanner(System.in);
        boolean validChoice = false;

        System.out.println("\n\nWelcome to Minesweeper!");

        System.out.println("\nCurrent game types");
        System.out.println("1 - Premade");
        System.out.println("2 - Custom");
        System.out.print("Select a game type: ");

        while (!validChoice) { 
            if(scanner.hasNextInt()){
                int choice = scanner.nextInt();
                switch (choice) {
                    case 1:
                        difficulty(game);
                        validChoice = true;
                        break;
                    case 2:
                        custom(game);
                        validChoice = true;
                        break;
                    default:
                        System.out.print("Invalid choice. Please select 1 or 2: ");
                }
            } else {
                System.out.print("Please enter a valid number: ");
                scanner.next();
            }
        }
        playMinesweeper(game);
        scanner.close();
    }

    public static void main(String[] args) {
        gameIntro();
    }


}
