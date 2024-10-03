package main

import (
	"bufio"
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"strings"
	"time"
)

// Constants for the game size
const MAXSIDE int = 25
const MAXMINES int = 99

// Sweeper structure
type Sweeper struct {
	side  int
	mines int
}

// Offsets for checking neighbors (same as rowOffsets and colOffsets in C)
var rowOffsets = []int{-1, -1, 0, 1, 1, 1, 0, -1}
var colOffsets = []int{0, 1, 1, 1, 0, -1, -1, -1}

// Checks if location on board is valid (not outside board or not treated as gridpoint)
func checkSpot(row, col, side int) bool {
	return row < side && col < side && row >= 0 && col >= 0
}

// Checks if a mine is present at a location
func checkMine(row, col int, board [][]rune) bool {
	return board[row][col] == '*'
}

// Sets the board to be displayed to the player
func setBoard(realBoard, shownBoard [][]rune, side int) {
	for i := 0; i < side; i++ {
		for j := 0; j < side; j++ {
			shownBoard[i][j] = '-'
			realBoard[i][j] = '-'
		}
	}
}

// Places mines around the board
func placeMines(mineStorage [][]int, realBoard [][]rune, mines, side, firstRow, firstCol int) {
	rand.NewSource(time.Now().UnixNano())
	checkOpenSpot := make([]bool, side*side)

	for i := 0; i < mines; {
		randRow := rand.Intn(side)
		randCol := rand.Intn(side)

		if (randRow == firstRow && randCol == firstCol) || checkOpenSpot[randRow*side+randCol] {
			continue
		}

		mineStorage[i][0] = randRow
		mineStorage[i][1] = randCol
		realBoard[randRow][randCol] = '*'
		checkOpenSpot[randRow*side+randCol] = true
		i++
	}
}

// Displays the board
func displayBoard(board [][]rune, side int) {
	fmt.Printf("   ")

	for i := 0; i < side; i++ {
		fmt.Printf("%d ", i+1)
	}
	fmt.Println()

	for i := 0; i < side; i++ {
		fmt.Printf("%d  ", i+1)
		for j := 0; j < side; j++ {
			fmt.Printf("%c ", board[i][j])
		}
		fmt.Println()
	}
}

// Counts mines to ensure they are properly implemented
func countMines(row, col int, realBoard [][]rune, side int) int {
	count := 0
	for i := 0; i < 8; i++ {
		newRow := row + rowOffsets[i]
		newCol := col + colOffsets[i]
		if checkSpot(newRow, newCol, side) && checkMine(newRow, newCol, realBoard) {
			count++
		}
	}
	return count
}

// Checks if player has clicked on a mine or not
func calculateMove(row, col *int, movesLeft *int, shownBoard, realBoard [][]rune, mineStorage [][]int, mines, side int) bool {
	if shownBoard[*row][*col] != '-' {
		return false
	}

	if realBoard[*row][*col] == '*' {
		shownBoard[*row][*col] = '*'

		for i := 0; i < mines; i++ {
			shownBoard[mineStorage[i][0]][mineStorage[i][1]] = '*'
		}

		fmt.Print("\nFinal Status of Board:\n\n")
		displayBoard(shownBoard, side)

		fmt.Println("\nYou lost!")
		return true
	}

	mineCount := countMines(*row, *col, realBoard, side)
	(*movesLeft)--
	shownBoard[*row][*col] = rune(mineCount + '0')

	if mineCount == 0 {
		for i := 0; i < 8; i++ {
			newRow := *row + rowOffsets[i]
			newCol := *col + colOffsets[i]

			if checkSpot(newRow, newCol, side) && !checkMine(newRow, newCol, realBoard) {
				calculateMove(&newRow, &newCol, movesLeft, shownBoard, realBoard, mineStorage, mines, side)
			}
		}
	}
	return false
}

// Gets numerical input from user
func numInput() int {
	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		input := scanner.Text()
		if val, err := strconv.Atoi(strings.TrimSpace(input)); err == nil {
			return val
		} else {
			fmt.Println("Invalid input number. Please try again: ")
		}
	}
	return 0
}

// Gets character input from user
func charInput() rune {
	reader := bufio.NewReader(os.Stdin)
	input, _ := reader.ReadString('\n')
	return rune(strings.ToUpper(strings.TrimSpace(input))[0])
}

// Player perspective of game
func playMinesweeper(game *Sweeper) {
	gameOver := false
	quitGame := false
	currentMoveIndex := 0
	movesLeft := game.side*game.side - game.mines
	var x, y int

	mineStorage := make([][]int, MAXSIDE*MAXSIDE)
	for i := range mineStorage {
		mineStorage[i] = make([]int, 2)
	}

	realBoard := make([][]rune, game.side)
	shownBoard := make([][]rune, game.side)
	for i := range realBoard {
		realBoard[i] = make([]rune, game.side)
		shownBoard[i] = make([]rune, game.side)
	}

	setBoard(realBoard, shownBoard, game.side)

	for !gameOver {
		fmt.Print("\nCurrent Status of Board:\n\n")
		displayBoard(shownBoard, game.side)
		boardEdit := false

		fmt.Println("\nEnter an action to do (type 'H' to see available actions): ")

		for !boardEdit {
			action := charInput()
			if action == ' ' {
				continue
			}

			if action == 'C' || action == 'M' {
				fmt.Print("\nEnter the column position (x - axis) -> ")
				y = numInput() - 1

				fmt.Print("Enter the row position (y - axis) -> ")
				x = numInput() - 1

				fmt.Println()

				if x < 0 || x >= game.side || y < 0 || y >= game.side {
					fmt.Println("Invalid move. Please try again.")
					continue
				}
				boardEdit = true
			}

			// Check a tile
			if action == 'C' {
				fmt.Println("Checking tile...")
				if shownBoard[x][y] != '-' {
					fmt.Println("The cell has already been opened or marked. Please try again.")
					continue
				}

				if currentMoveIndex == 0 {
					placeMines(mineStorage, realBoard, game.mines, game.side, x, y)
				}

				currentMoveIndex++
				gameOver = calculateMove(&x, &y, &movesLeft, shownBoard, realBoard, mineStorage, game.mines, game.side)

				if !gameOver && movesLeft == 0 {
					fmt.Print("\nFinal Status of Board:\n\n")
					for i := 0; i < game.mines; i++ {
						shownBoard[mineStorage[i][0]][mineStorage[i][1]] = '@'
					}
					displayBoard(shownBoard, game.side)
					fmt.Println("\nYou won!")
					gameOver = true
				}

			} else if action == 'M' {
				if shownBoard[x][y] == '-' {
					shownBoard[x][y] = '@'
				} else if shownBoard[x][y] == '@' {
					shownBoard[x][y] = '-'
				} else {
					fmt.Println("Cannot mark this tile. Please try again.")
					continue
				}

			} else if action == 'H' {
				fmt.Println("\nAvailable actions:")
				fmt.Println("C - Check a tile")
				fmt.Println("M - Mark/Unmark a tile")
				fmt.Println("H - See available actions")
				fmt.Println("Q - Quit the game")
				fmt.Print("\nPlease select one of the available actions: ")

			} else if action == 'Q' {
				fmt.Println("\nQuitting the game...")
				quitGame = true
				return
			} else {
				fmt.Print("Invalid action. Please try again: ")
			}

			if quitGame {
				break
			}
		}
	}

	fmt.Print("\nWould you like to play again? (Y/N): ")
	for !quitGame {
		action := charInput()

		if action == 'Y' {
			gameIntro()
			quitGame = true
		} else if action == 'N' {
			fmt.Print("\n\nThanks for playing!\n\n")
			quitGame = true
		} else {
			fmt.Print("Invalid input. Please try again: ")
		}
	}
}

func difficulty(game *Sweeper) {
	var level int
	reader := bufio.NewReader(os.Stdin)
	loop := true

	fmt.Println("\n\nCurrent difficulty levels")
	fmt.Println("1 - Beginner (9x9 grid, 10 mines)")
	fmt.Println("2 - Intermediate (16x16 grid, 40 mines)")
	fmt.Println("3 - Advanced (23x23 grid, 99 mines)")

	fmt.Println("\nSelect a difficulty level: ")

	for loop {
		input, _ := reader.ReadString('\n')
		input = strings.TrimSpace(input)
		level, _ = strconv.Atoi(input)

		switch level {
		case 1:
			game.side = 9
			game.mines = 10
			loop = false
		case 2:
			game.side = 16
			game.mines = 40
			loop = false
		case 3:
			game.side = 23
			game.mines = 99
			loop = false
		default:
			fmt.Println("Invalid choice. Please select 1, 2, or 3: ")
		}
	}
}

func custom(game *Sweeper) {
	reader := bufio.NewReader(os.Stdin)
	loop := true

	fmt.Println("\n\nCustom difficulty selected!")
	fmt.Println("\nEnter the amount of sides (maximum is 25, minimum is 2): ")

	for loop {
		input, _ := reader.ReadString('\n')
		input = strings.TrimSpace(input)
		sides, err := strconv.Atoi(input)

		if err == nil && sides >= 2 && sides <= 25 {
			game.side = sides
			loop = false
		} else {
			fmt.Print("Invalid amount. Please enter a value between 1 and 26: ")
		}
	}

	maxMines := game.side*game.side - 1
	if maxMines > 99 {
		maxMines = 99
	}

	fmt.Printf("\nEnter the amount of mines (maximum is %d, minimum is 1): ", maxMines)

	for loop {
		input, _ := reader.ReadString('\n')
		input = strings.TrimSpace(input)
		mines, err := strconv.Atoi(input)

		if err == nil && mines >= 1 && mines <= maxMines {
			game.mines = mines
			loop = false
		} else {
			fmt.Printf("Invalid amount. Please enter a value between 1 and %d: ", maxMines)
			continue
		}
	}
}

func gameIntro() {
	var game Sweeper
	var input int
	reader := bufio.NewReader(os.Stdin)
	loop := true

	fmt.Println("\n\nWelcome to my Minesweeper game!")

	fmt.Println("\nCurrent game types")
	fmt.Println("1 - Premade")
	fmt.Println("2 - Custom")

	fmt.Print("\nSelect a game type: ")

	for loop {
		inputStr, _ := reader.ReadString('\n')
		inputStr = strings.TrimSpace(inputStr)
		input, _ = strconv.Atoi(inputStr)

		switch input {
		case 1:
			difficulty(&game)
			loop = false
		case 2:
			custom(&game)
			loop = false
		default:
			fmt.Print("Invalid choice. Please select 1 or 2: ")
		}
	}

	playMinesweeper(&game)
}

func main() {
	gameIntro()
}
