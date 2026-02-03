#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include "header.h"

long micro_seconds(const struct timespec *start, const struct timespec *stop) {
    long sec  = stop->tv_sec  - start->tv_sec;
    long nsec = stop->tv_nsec - start->tv_nsec;
    return sec * 1000000L + nsec / 1000L;
}

int count = 0;

void solve(int col, int **board){
    
    if(col == N){
        #pragma omp atomic
        count++;
            // #pragma omp critical
            // {    
            //     printboard(board);
            // }
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

    #pragma omp parallel for reduction (+:count) 
    for(int row = 0; row < N; row++){
        int **local_b = initboard();
        local_b[row][0] = 1;
        solve(1, local_b);
        free(local_b);
    }

    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = micro_seconds(&t_start, &t_stop);

    printf("total count: %i\n", count);

    printf("WALL TIME (omp): %ld microseconds\n", wall);
}