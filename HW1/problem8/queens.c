#include <stdio.h>
#include <pthread.h>

#define N 8

void solve(int col, int **board){

    if(col == N){
        printboard();
        return;
    }

    for(int row = 0; row < N; row++){
        if safe(row, col, board){
            board[row][col] = 1;

            solve(col+1, board);
            
            board[row][col] = 0;
        }
    }

    


}

int main(){

}