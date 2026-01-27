#include <pthread.h>
#include <stdio.h>
#include <omp.h>

#include "board.h"
#include "safe.h"

#define N 8

int count = 0;

void solve(int col, int **board){
    
    if(col == N){
        #pragma omp critical
        {
            count++;
            printf("\ncount: %i \n", count);
            printboard(board);
            return;
        }
        
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
    #pragma omp parallel for reduction (+:count)
    for(int row = 0; row < N; row++){
        int **local_b = initboard();
        local_b[row][0] = 1;
        solve(1, local_b);
    }
    
    printf("total count: %i\n", count);
}