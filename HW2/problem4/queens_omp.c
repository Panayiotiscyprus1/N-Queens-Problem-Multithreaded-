#include <pthread.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

#include "board.h"
#include "safe.h"

#define N 8

long micro_seconds(const struct timespec *start, const struct timespec *stop) {
    long sec  = stop->tv_sec  - start->tv_sec;
    long nsec = stop->tv_nsec - start->tv_nsec;
    return sec * 1000000L + nsec / 1000L;
}

int count = 0;
void solve(int col, int **board){
    #pragma omp parallel
    {
        if(col == N){
            #pragma omp critical
            {
                count++;
                printf("\ncount: %i \n", count);
                printboard(board);
            }
            return;
        }

        #pragma omp parallel for
        for(int row = 0; row < N; row++){
            if (safe(row, col, board)){
                board[row][col] = 1;

                solve(col+1, board);
                
                board[row][col] = 0;
            }
        }
    }

}

int main(){
    struct timespec t_start, t_stop;

    int **b = initboard();

    clock_gettime(CLOCK_MONOTONIC, &t_start);

    solve(0, b);
    printf("total count: %i\n", count);

    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = micro_seconds(&t_start, &t_stop);

    printf("WALL TIME: %ld microseconds\n", wall);
}