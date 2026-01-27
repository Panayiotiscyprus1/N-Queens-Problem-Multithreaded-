#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int** initboard() {
    int **board = malloc(N * sizeof(int*));

    for (int i = 0; i < N; i++) {
        board[i] = malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            board[i][j] = 0;  // initialize values
        }
    }
    return board;
}

void printboard(int **board){
    printf("\n|---|---|---|---|---|---|---|---|\n");
    for (int i = 0; i < N; i++) {
        printf("| ");
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 1)
                printf("♛ | ");
            else
            printf("  | ");
        }
        printf("\n|---|---|---|---|---|---|---|---|\n");
    }
}