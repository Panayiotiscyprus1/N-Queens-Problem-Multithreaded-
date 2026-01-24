#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define N 8

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
    for (int i = 0; i < 8; i++) {
        printf("| ");
        for (int j = 0; j < 8; j++) {
            printf("%i | ", board[i][j]);
        }
        printf("\n|---|---|---|---|---|---|---|---|\n");
    }
}


void solve(int col, int **board){

    if(col == N){
        printboard(board);
        return;
    }

    for(int row = 0; row < N; row++){
        if (safe(row, col, board)){
            board[row][col] = 1;

            solve(col+1, board);
            
            board[row][col] = 0;
        }
    }

}


int main(){
    int **b = initboard();
    printboard(b);
}