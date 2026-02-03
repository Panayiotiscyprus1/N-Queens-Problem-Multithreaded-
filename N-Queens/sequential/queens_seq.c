#include <pthread.h>
#include <stdio.h>
#include "header.h"


long micro_seconds(const struct timespec *start, const struct timespec *stop) {
    long sec  = stop->tv_sec  - start->tv_sec;
    long nsec = stop->tv_nsec - start->tv_nsec;
    return sec * 1000000L + nsec / 1000L;
}

int count = 0;
void solve(int col, int **board){

    if(col == N){
        count++;
        // printf("\ncount: %i \n", count);
        // printboard(board);
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
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    int **b = initboard();
    solve(0, b);

    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = micro_seconds(&t_start, &t_stop);

    printf("total count: %i\n", count);

    printf("WALL TIME (sequential): %ld microseconds\n", wall);

}