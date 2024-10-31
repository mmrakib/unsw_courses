// Assignment 1 21T2 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by MOHAMMAD MAYAZ RAKIB (z5361151)
// on 11/7/2021
//
// Version 1.0.0 (2021-06-19): Assignment released.

#include <stdio.h>
#include <stdlib.h>

// Possible square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

// The size of the starting grid.
#define SIZE              8

// The possible command codes.
#define DETECT_ROW      1
#define DETECT_SQUARE   2
#define REVEAL_CROSS    3
#define GAME_MODE       4
#define FLAG_MINE       5
#define DEFUSE          6

// Add any extra #defines here.
#define MAX_ARGS_COUNT 5

#define TRUE 1
#define FALSE 0

#define DEBUG_MODE 0
#define GAMEPLAY_MODE 1

// Place your function prototyes here.
void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
void print_debug_minefield_lost(int minefield[SIZE][SIZE]);
void print_gameplay_minefield(int minefield[SIZE][SIZE]);
void print_gameplay_minefield_lost(int minefield[SIZE][SIZE]);

void error(char *message);
int check_bounds(int row, int col);
int detect_adjacent(int minefield[SIZE][SIZE], int row, int col);
int game_won(int minefield[SIZE][SIZE]);

int main(void) {
    int minefield[SIZE][SIZE];

    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    printf("How many mines? ");

    // Coordinate holders
    int x, y;

    // TODO: Scan in the number of pairs of mines.
    int num_pairs = 0;
    scanf("%d", &num_pairs);

    printf("Enter pairs:\n");

    // TODO: Scan in the pairs of mines and place them on the grid.
    x = 0; 
    y = 0;
    for (int i = 0; i < num_pairs; i++) {
        scanf("%d %d", &x, &y);

        // Checks for invalid input outside of minefield
        if (check_bounds(x, y)) {
            minefield[x][y] = HIDDEN_MINE;
        }
    }

    printf("Game Started\n");
    print_debug_minefield(minefield);

    // TODO: Scan in commands to play the game until the game ends.
    // A game ends when the player wins, loses, or enters EOF (Ctrl+D).
    // You should display the minefield after each command has been processed.

    // Gamemode state
    int gameplay_mode = DEBUG_MODE;
    // Starts with 3 hints
    int hints = 3;

    int command;
    while (scanf("%d", &command) == 1) {
        if (command == 1) {
            int row, col, num_squares;

            scanf("%d %d %d", &row, &col, &num_squares);

            // Checks for invalid input outside the gamefield
            if (!check_bounds(row, col) || (col + num_squares) > SIZE) {
                printf("Coordinates not on map\n");

                if (gameplay_mode == DEBUG_MODE) {
                    print_debug_minefield(minefield);
                } else {
                    print_gameplay_minefield(minefield);
                }
            } else {
                if (hints > 0) {
                    hints--;

                    // Scans line for HIDDEN_MINE squares
                    int num_mines = 0;
                    for (int i = col; i < (col + num_squares); i++) {
                        if (minefield[row][i] == HIDDEN_MINE) {
                            num_mines++;
                        }
                    }

                    // Formatted printing of result
                    printf("There are %d mine(s) in row %d, from column %d to %d\n", 
                    num_mines, row, col, col + num_squares - 1);

                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield(minefield);
                    }

                } else {
                    // Doesn't run command if all hints have been used
                    printf("Help already used\n");

                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield(minefield);
                    }
                }
            }
        }

        if (command == 2) {
            int row, col, num_squares;

            scanf("%d %d %d", &row, &col, &num_squares);

            // Checks for invalid input outside of gamefield
            if (!check_bounds(row, col)) {
                printf("Coordinates not on map\n");

                if (gameplay_mode == DEBUG_MODE) {
                    print_debug_minefield(minefield);
                } else {
                    print_gameplay_minefield(minefield);
                }
            } else {
                if (hints > 0) {
                    hints--;

                    // Sets radius to half of the number of squares
                    int radius = num_squares / 2; 
                    // Starts from the top-left of the scanned squares
                    int start_row = row - radius; 
                    int start_col = col - radius; 

                    int num_mines = 0;
                    // Nested loop to go through all rows and columns of square
                    for (int i = start_col; i < (start_col + num_squares); i++) {
                        for (int j = start_row; j < (start_row + num_squares); j++) {
                            // Checks if scanned squares are within the gamefield, 
                            // and only checks for mine if they are
                            if (check_bounds(j, i)) {
                                if (minefield[j][i] == HIDDEN_MINE) {
                                    num_mines++;
                                }
                            }
                        }
                    }

                    // Formatted printing of output
                    printf("There are %d mine(s) in the square "
                    "centered at row %d, column %d of size %d\n",
                        num_mines, row, col, num_squares);

                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield(minefield);
                    }

                } else {
                    // Doesn't run command if all hints have been used
                    printf("Help already used\n");

                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield(minefield);
                    }
                }
            }
        }

        if (command == 3) {
            int row, col;

            scanf("%d %d", &row, &col);

            // Checks for invalid input outside of gamefield
            if (!check_bounds(row, col)) {
                printf("Coordinates not on map\n");

                if (gameplay_mode == DEBUG_MODE) {
                    print_debug_minefield(minefield);
                } else {
                    print_gameplay_minefield(minefield);
                }
            } else if (minefield[row][col] == HIDDEN_MINE) {
                // Checks loss condition
                if (gameplay_mode == DEBUG_MODE) {
                    print_debug_minefield_lost(minefield);
                } else {
                    print_gameplay_minefield_lost(minefield);
                }

                break;
            } else {

                // Selected square is revealed
                minefield[row][col] = VISIBLE_SAFE;

                // Left square is revealed only if no adjacent mines
                if (check_bounds(row, col - 1)) {
                    if (minefield[row][col - 1] != HIDDEN_MINE 
                        && detect_adjacent(minefield, row, col - 1) == 0) {
                        minefield[row][col - 1] = VISIBLE_SAFE;
                    }
                }

                // Top square is revealed only if no adjacent mines
                if (check_bounds(row - 1, col)) {
                    if (minefield[row - 1][col] != HIDDEN_MINE 
                        && detect_adjacent(minefield, row - 1, col) == 0) {
                        minefield[row - 1][col] = VISIBLE_SAFE;
                    }
                }

                // Right square is revealed only if no adjacent mines
                if (check_bounds(row, col + 1)) {
                    if (minefield[row][col + 1] != HIDDEN_MINE 
                        && detect_adjacent(minefield, row, col + 1) == 0) {
                        minefield[row][col + 1] = VISIBLE_SAFE;
                    }
                }

                // Bottom square is revealed only if no adjacent mines
                if (check_bounds(row + 1, col)) {
                    if (minefield[row + 1][col] != HIDDEN_MINE 
                        && detect_adjacent(minefield, row + 1, col) == 0) {
                        minefield[row + 1][col] = VISIBLE_SAFE;
                    }
                }

                if (game_won(minefield) == TRUE) {
                    // Checks win condition
                    printf("Game Won!\n");

                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield(minefield);
                    }
                    break;
                } else {
                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield(minefield);
                    }
                }
            }
        }

        if (command == 4) {
            // Toggles between DEBUG and GAMEPLAY modes
            if (gameplay_mode == DEBUG_MODE) {
                gameplay_mode = GAMEPLAY_MODE;
                printf("Gameplay mode activated\n");
                print_gameplay_minefield(minefield);
            } else {
                gameplay_mode = DEBUG_MODE;
                printf("Debug mode activated\n");
                print_debug_minefield(minefield);
            }
        }
    
        if (game_won(minefield) == TRUE) {
            // Checks win condition
            printf("Game Won!\n");

            if (gameplay_mode == DEBUG_MODE) {
                print_debug_minefield(minefield);
            } else {
                print_gameplay_minefield(minefield);
            }
        }
    } 

    return 0;
}


// Set the entire minefield to HIDDEN_SAFE.
void initialise_field(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            minefield[i][j] = HIDDEN_SAFE;
            j++;
        }
        i++;
    }
}


// Print out the actual values of the minefield.
void print_debug_minefield(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", minefield[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

// Print out values of minefield after losing, with extra text
void print_debug_minefield_lost(int minefield[SIZE][SIZE]) {
    printf("Game over\n");
    //printf("xx\n/\\\n");

    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", minefield[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

// Print out values of minefield in GAMEPLAY mode
void print_gameplay_minefield(int minefield[SIZE][SIZE]) {
    printf("..\n\\/\n");

    printf("    00 01 02 03 04 05 06 07\n");
    printf("   -------------------------\n");
    for (int i = 0; i < SIZE; i++) {
        printf("0%d |", i);
        for (int j = 0; j < SIZE; j++) {
            int num_mines = detect_adjacent(minefield, i, j);

            if (minefield[i][j] == HIDDEN_SAFE || minefield[i][j] == HIDDEN_MINE) {
                printf("##");
            } else if (minefield[i][j] == VISIBLE_SAFE && num_mines <= 0) {
                printf("  ");
            } else if (minefield[i][j] == VISIBLE_SAFE && num_mines > 0) {
                printf("0%d", num_mines);
            }

            if (j < 7) {
                printf(" ");
            }
        }
        printf("|\n");
    }
    printf("   -------------------------\n");
}

// Print out values of minefield in GAMEPLAY mode after losing, with extra text
void print_gameplay_minefield_lost(int minefield[SIZE][SIZE]) {
    printf("Game over\n");
    printf("xx\n/\\\n");

    printf("    00 01 02 03 04 05 06 07\n");
    printf("   -------------------------\n");
    for (int i = 0; i < SIZE; i++) {
        printf("0%d |", i);
        for (int j = 0; j < SIZE; j++) {
            int num_mines = detect_adjacent(minefield, i, j);

            if (minefield[i][j] == HIDDEN_SAFE) {
                printf("##");
            } else if (minefield[i][j] == HIDDEN_MINE) {
                printf("()");
            } else if (minefield[i][j] == VISIBLE_SAFE && num_mines <= 0) {
                printf("  ");
            } else if (minefield[i][j] == VISIBLE_SAFE && num_mines > 0) {
                printf("0%d", num_mines);
            }

            if (j < 7) {
                printf(" ");
            }
        }
        printf("|\n");
    }
    printf("   -------------------------\n");
}

// Prints error message
void error(char *message) {
    printf("%s\n", message);
}

// Checks if (row,col) pair is within SIZExSIZE board
int check_bounds(int row, int col) {
    if ((row >= 0 && row < SIZE) 
    && (col >= 0 && col < SIZE)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// Detects number of mines on adjacent tiles
int detect_adjacent(int minefield[SIZE][SIZE], int row, int col) {
    int start_row = row - 1;
    int start_col = col - 1;

    int num_mines = 0;
    for (int i = start_col; i < (start_col + 3); i++) {
        for (int j = start_row; j < (start_row + 3); j++) {
            if (check_bounds(j, i) && !(i == col && j == row)) {
                if (minefield[j][i] == HIDDEN_MINE) {
                    num_mines++;
                }
            }
        }
    }

    return num_mines;
}

// Checks if game is won
int game_won(int minefield[SIZE][SIZE]) {
    int i = 0;
    int won = TRUE;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            if (minefield[i][j] == HIDDEN_SAFE) {
                won = FALSE;
            }
            j++;
        }
        i++;
    }
    return won;
}