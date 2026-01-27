<<<<<<< HEAD:safe.c
#include "header.h"
=======
#define N 8
>>>>>>> abfba509741475083afd22b10a04f052d8bcea9d:HW1/problem7/safe.c

int safe(int row, int col, int **board){
    // Check this row on left side
    for(int j=col; j>=0; j--){
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