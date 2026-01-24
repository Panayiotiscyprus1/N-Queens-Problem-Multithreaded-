#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define N 8

int safe(int row, int col, int **board);

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

int safe(int row, int col, int **board){
    // Check this row on left side
    for(int j=col; j>0; j--){
        if(board[row][j] == 1) return 0;
    }
    // Check upper diagonal on left side
    for(int i=row, j=col; i>=0 && j>=0; i--, j--){
        if(board[i][j]==1) return 0;
    }
    // Check lower diagonal on left side
    for(int i=row, j=col; i<N && j>=0; i++, j--){
        if(board[i][j]==1) return 0;
    }
    return 1;
}


int main(){
    int **b = initboard();
    printboard(b);
}