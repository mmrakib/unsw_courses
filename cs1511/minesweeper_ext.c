// Assignment 1 21T2 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by MOHAMMAD MAYAZ RAKIB (z5361151)
// on 7/7/2021
//
// Version 1.0.0 (2021-06-19): Assignment released.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Possible square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

#define FLAGGED_SAFE    3
#define FLAGGED_MINE    4

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
#define DEFUSE_RADIUS 2

#define TRUE 1
#define FALSE 0

#define DEBUG_MODE 0
#define GAMEPLAY_MODE 1

void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
void print_gameplay_minefield(int minefield[SIZE][SIZE]);
void print_gameplay_minefield_lost(int minefield[SIZE][SIZE]);

// Place your function prototyes here.
void error(char *message);
int check_bounds(int row, int col);
int detect_adjacent(int minefield[SIZE][SIZE], int row, int col);
int detect_flags(int minefield[SIZE][SIZE], int row, int col);
int check_lost(int minefield[SIZE][SIZE], int row, int col);
void defuse(int minefield[SIZE][SIZE], int row, int col);
void defuse_second(int minefield[SIZE][SIZE], int row, int col);

int main(void) {
    int minefield[SIZE][SIZE];

    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    printf("How many mines? ");

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
        if (check_bounds(x,y)) {
            minefield[x][y] = HIDDEN_MINE;
        } else {
            //error("Placing mine outside minefield");
        }
    }

    printf("Game Started\n");
    print_debug_minefield(minefield);

    int gameplay_mode = DEBUG_MODE;
    int hints = 3; 

    int command;
    while (scanf("%d", &command) == 1) {
        if (command == DETECT_ROW) {
            int row, col, num_squares;

            scanf("%d %d %d", &row, &col, &num_squares);

     
            if (!check_bounds(row,col) || (col + num_squares) > SIZE) {
                printf("Coordinates not on map\n");
            } else {
                if (hints > 0) {
                    hints--;

                    int num_mines = 0;
                    for (int i = col; i < (col + num_squares); i++) {
                        if (minefield[row][i] == HIDDEN_MINE) {
                            num_mines++;
                        }
                    }

                    printf("There are %d mine(s) in row %d, from column %d to %d\n", num_mines, row, 
                    col, col + num_squares - 1);

                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield(minefield);
                    }


                } else {
                    printf("Help already used\n");

                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield(minefield);
                    }
                }
            }
        }

        if (command == DETECT_SQUARE) {
            int row, col, num_squares;

            scanf("%d %d %d", &row, &col, &num_squares);

            if (!check_bounds(row,col)) {
                printf("Coordinates not on map\n");
            } else {
                if (hints > 0) {
                    hints--;

                    int radius = num_squares / 2;
                    int start_row = row - radius; //printf("Start row: %d\n", start_row);
                    int start_col = col - radius; //printf("Start col: %d\n", start_col);

                    int num_mines = 0;
                    for (int i = start_col; i < (start_col + num_squares); i++) {
                        for (int j = start_row; j < (start_row + num_squares); j++) {
                            //if ((i >= 0 && i < SIZE) && (j >= 0 && j < SIZE)) {
                            if (check_bounds(j,i)) {
                                //printf("(%d, %d) ", j, i);
                                if (minefield[j][i] == HIDDEN_MINE) {
                                    num_mines++;
                                }
                                //printf("\n");
                            }
                        }
                    }

                    printf("There are %d mine(s) in the square centered at row %d, column %d, of size %d\n",
                        num_mines, row, col, num_squares);

                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield(minefield);
                    }

                } else {
                    printf("Help already used\n");

                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield(minefield);
                    }
                }
            }
        }

        if (command == REVEAL_CROSS) {
            int row, col;

            scanf("%d %d", &row, &col);

            if (!check_bounds(row,col)) {
                printf("Coordinates not on map\n");
            }

            if (minefield[row][col] == FLAGGED_SAFE
                || minefield[row][col] == FLAGGED_MINE) {
                //printf("Cannot reveal at flagged square\n")
            } else {
                // Win condition check
                if (check_lost(minefield, row, col) == TRUE) {
                    //printf("%d\n", minefield[row][col]);
                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield_lost(minefield);
                    }
                    break;
                }

                minefield[row][col] = VISIBLE_SAFE;

                int game_lost = FALSE;

                if (check_bounds(row, col-1)) {
                    int num_mines = detect_adjacent(minefield, row, col-1);
                    if (minefield[row][col-1] == HIDDEN_MINE || minefield[row][col-1] == FLAGGED_MINE) {
                        num_mines++;
                    }
                    /*printf("minefield[row][col-1] == HIDDEN_MINE: %d\n", minefield[row][col-1] == HIDDEN_MINE);
                    printf("minefield[row][col-1] == FLAGGED_MINE: %d\n", minefield[row][col-1] == FLAGGED_MINE);
                    printf("row: %d col: %d\n", row, col-1);
                    printf("detect_adjacent: %d\n",detect_adjacent(minefield, row, col-1));
                    printf("detect_flags: %d\n",detect_flags(minefield, row, col-1));*/

                    if (/*minefield[row][col-1] != HIDDEN_MINE && minefield[row][col-1] != FLAGGED_MINE
                        &&*/ num_mines == detect_flags(minefield, row, col-1)) {
                        //minefield[row][col-1] = VISIBLE_SAFE;
                        //printf("check_lost: %d\n", check_lost(minefield, row, col-1, gameplay_mode));
                        if (check_lost(minefield, row, col-1) == TRUE) {
                            game_lost = TRUE;
                        } else {
                            minefield[row][col-1] = VISIBLE_SAFE;
                        }
                    }


                }

                if (check_bounds(row-1, col)) {
                    int num_mines = detect_adjacent(minefield, row-1, col);
                    if (minefield[row-1][col] == HIDDEN_MINE || minefield[row-1][col] == FLAGGED_MINE) {
                        num_mines++;
                    }

                    if (/*minefield[row-1][col] != HIDDEN_MINE && minefield[row-1][col] != FLAGGED_MINE
                        &&*/ num_mines == detect_flags(minefield, row-1, col)) {
                        //minefield[row-1][col] = VISIBLE_SAFE;

                        if (check_lost(minefield, row-1, col) == TRUE) {
                            game_lost = TRUE;
                        } else {
                            minefield[row-1][col] = VISIBLE_SAFE;
                        }
                    }
                }

                if (check_bounds(row, col+1)) {
                    int num_mines = detect_adjacent(minefield, row, col+1);
                    if (minefield[row][col+1] == HIDDEN_MINE || minefield[row][col+1] == FLAGGED_MINE) {
                        num_mines++;
                    }

                    if (/*minefield[row][col+1] != HIDDEN_MINE && minefield[row][col+1] != FLAGGED_MINE 
                        &&*/ num_mines == detect_flags(minefield, row, col+1)) {
                        //minefield[row][col+1] = VISIBLE_SAFE;

                        if (check_lost(minefield, row, col+1) == TRUE) {
                            game_lost = TRUE;
                        } else {
                            minefield[row][col+1] = VISIBLE_SAFE;
                        }
                    }
                }

                if (check_bounds(row+1, col)) {
                    int num_mines = detect_adjacent(minefield, row+1, col);
                    if (minefield[row+1][col] == HIDDEN_MINE || minefield[row+1][col] == FLAGGED_MINE) {
                        num_mines++;
                    }

                    if (/*minefield[row+1][col] != HIDDEN_MINE && minefield[row+1][col] != FLAGGED_MINE
                        &&*/ num_mines == detect_flags(minefield, row+1, col)) {
                        //minefield[row+1][col] = VISIBLE_SAFE;

                        if (check_lost(minefield, row+1, col) == TRUE) {
                            game_lost = TRUE;
                        } else {
                            minefield[row+1][col] = VISIBLE_SAFE;
                        }
                    }
                }

                if (game_lost == TRUE) {
                    if (gameplay_mode == DEBUG_MODE) {
                        print_debug_minefield(minefield);
                    } else {
                        print_gameplay_minefield_lost(minefield);
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

        if (command == GAME_MODE) {
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

        if (command == FLAG_MINE) {
            int row, col;

            scanf("%d %d", &row, &col);

            //if (row >= SIZE || col >= SIZE) {
            if (!check_bounds(row,col)) {
                printf("Coordinates not on map");
            }

            if (minefield[row][col] == VISIBLE_SAFE) {

            } else if (minefield[row][col] == HIDDEN_SAFE) {
                minefield[row][col] = FLAGGED_SAFE;
                printf("Flag placed at row %d, column %d\n", row, col);
                if (gameplay_mode == DEBUG_MODE) {
                    print_debug_minefield(minefield);
                } else {
                    print_gameplay_minefield(minefield);
                }
            } else if (minefield[row][col] == HIDDEN_MINE) {
                minefield[row][col] = FLAGGED_MINE;
                printf("Flag placed at row %d, column %d\n", row, col);
                if (gameplay_mode == DEBUG_MODE) {
                    print_debug_minefield(minefield);
                } else {
                    print_gameplay_minefield(minefield);
                }
            } else if (minefield[row][col] == FLAGGED_SAFE) {
                minefield[row][col] = HIDDEN_SAFE;
                printf("Flag removed at row %d, column %d\n", row, col);
                if (gameplay_mode == DEBUG_MODE) {
                    print_debug_minefield(minefield);
                } else {
                    print_gameplay_minefield(minefield);
                }
            } else if (minefield[row][col] == FLAGGED_MINE) {
                minefield[row][col] = HIDDEN_MINE;
                printf("Flag removed at row %d, column %d\n", row, col);
                if (gameplay_mode == DEBUG_MODE) {
                    print_debug_minefield(minefield);
                } else {
                    print_gameplay_minefield(minefield);
                }
            }
        }

        if (command == DEFUSE) {
            int row, col;

            scanf("%d %d", &row, &col);

            //if (row >= SIZE || col >= SIZE) {
            if (!check_bounds(row,col)) {
                printf("Coordinates not on map");
            }

            defuse(minefield, row, col);

            if (gameplay_mode == DEBUG_MODE) {
                print_debug_minefield(minefield);
            } else {
                print_gameplay_minefield(minefield);
            }
        }
    }

    // TODO: Scan in commands to play the game until the game ends.
    // A game ends when the player wins, loses, or enters EOF (Ctrl+D).
    // You should display the minefield after each command has been processed.

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
            /*if (minefield[i][j] == FLAGGED_SAFE) {
                printf("%d ", HIDDEN_SAFE);
            } else if (minefield[i][j] == FLAGGED_MINE) {
                printf("%d ", HIDDEN_MINE);
            } else {*/
                printf("%d ", minefield[i][j]);
            //}
            j++;
        }
        printf("\n");
        i++;
    }
}

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
            } else if (minefield[i][j] == FLAGGED_SAFE || minefield[i][j] == FLAGGED_MINE) {
                printf("!!");
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
            } else if (minefield[i][j] == FLAGGED_SAFE || minefield[i][j] == FLAGGED_MINE) {
                printf("!!");
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
    if ((row >= 0 && row < SIZE) && (col >= 0 && col < SIZE)) {
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
            if (check_bounds(j,i) && !(i == col && j == row)) {
                if (minefield[j][i] == HIDDEN_MINE || minefield[j][i] == FLAGGED_MINE) {
                    num_mines++;
                }
            }
        }
    }

    return num_mines;
}

// Detects number of mines on adjacent tiles
int detect_flags(int minefield[SIZE][SIZE], int row, int col) {
    int start_row = row - 1;
    int start_col = col - 1;

    int num_flags = 0;
    for (int i = start_col; i < (start_col + 3); i++) {
        for (int j = start_row; j < (start_row + 3); j++) {
            if (check_bounds(j,i) && !(i == col && j == row)) {
                if (minefield[j][i] == FLAGGED_SAFE || minefield[j][i] == FLAGGED_MINE) {
                    num_flags++;
                }
            }
        }
    }

    return num_flags;
}

int check_lost(int minefield[SIZE][SIZE], int row, int col) {
    if (minefield[row][col] == HIDDEN_MINE || minefield[row][col] == FLAGGED_MINE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void defuse(int minefield[SIZE][SIZE], int row, int col) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            double circle = sqrt( pow((i - row), 2) + pow((j - col), 2) );

            if (circle <= DEFUSE_RADIUS) {
                if (minefield[i][j] == HIDDEN_MINE) {
                    minefield[i][j] = HIDDEN_SAFE;
                    defuse_second(minefield, i, j);
                } else if (minefield[i][j] == FLAGGED_MINE) {
                    minefield[i][j] = FLAGGED_SAFE;
                    defuse_second(minefield, i, j);
                }
            }
        }
    }
}

void defuse_second(int minefield[SIZE][SIZE], int row, int col) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            double circle = sqrt( pow((i - row), 2) + pow((j - col), 2) );

            if (circle <= DEFUSE_RADIUS) {
                if (minefield[i][j] == HIDDEN_MINE) {
                    minefield[i][j] = HIDDEN_SAFE;
                } else if (minefield[i][j] == FLAGGED_MINE) {
                    minefield[i][j] = FLAGGED_SAFE;
                }
            }
        }
    }
}