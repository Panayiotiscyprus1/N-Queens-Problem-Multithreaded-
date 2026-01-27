#include <stdio.h>
#include <stdlib.h>

int** initboard() {
    int **board = malloc(8 * sizeof(int*));

    for (int i = 0; i < 8; i++) {
        board[i] = malloc(8 * sizeof(int));
        for (int j = 0; j < 8; j++) {
            board[i][j] = 0;  // initialize values
        }
    }
    return board;
}

void printboard(int **board){
    printf("\n|---|---|---|---|---|---|---|---|\n");
    for (int i = 0; i < 8; i++) {
        printf("| ");
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == 1)
                printf("♛ | ");
            else
            printf("  | ");
        }
        printf("\n|---|---|---|---|---|---|---|---|\n");
    }
}