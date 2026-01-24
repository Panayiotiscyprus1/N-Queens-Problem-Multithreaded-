#include <stdio.h>
#include <pthread.h>

#define N 8

void solve(int row, int col, int **board){

    if(row == N){
        printboard();
        return;
    }

    if safe(row){
        board[row][col++] = 1;
    }

    solve(row+1, col, board);
    board[row][col] = 0;


}

int main(){

}