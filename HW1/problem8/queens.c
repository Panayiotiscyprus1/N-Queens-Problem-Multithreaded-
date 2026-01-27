#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "safe.h"
#include <sys/time.h>
#include <time.h>

#define N 8

pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

long micro_seconds(const struct timespec *start, const struct timespec *stop) {
    long sec  = stop->tv_sec  - start->tv_sec;
    long nsec = stop->tv_nsec - start->tv_nsec;
    return sec * 1000000L + nsec / 1000L;
}


int results[N];
int total = 0;


typedef struct {
    int id;
    int col;
    int row;
    int **board;
} solveArg;


void *solve(void *arg){

    solveArg *args = (solveArg *)arg; 
    int col = args->col;
    int row = args->row;
    int **board = args->board;
    int id = args->id;

    if(col == N){
        results[id]+=1;
        
        pthread_mutex_lock(&print_lock);
        
        printf("\n I am thread %i with count %i \n", id, results[id]);
        printboard(board);

        pthread_mutex_unlock(&print_lock);

        return(NULL);
    }

    for(int r = 0; r < N; r++){
        if (safe(r, col, board)){
            board[r][col] = 1;
            
            args->col = col + 1;
            args->board = board;
            solve(args);
            board[r][col] = 0;
        }
    }
    return(NULL);
}

int main(){
    struct timespec t_start, t_stop;

    solveArg **arg = malloc(N*sizeof(*arg));

    for(int i = 0; i < N; i++){
        arg[i] = malloc(sizeof(*arg[i]));
        arg[i]->id = i;
        arg[i]->col = 1;
        arg[i]->row = i;
        arg[i]->board = initboard();
        arg[i]->board[i][0] = 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &t_start);

    pthread_t tid[N];
    for(long i = 0; i < N; i++){ 
        pthread_create(&tid[i], NULL, solve, arg[i]); 
    }

    for(int i = 0; i < N; i++){
        pthread_join(tid[i], NULL);
        total += results[i];
    }

    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = micro_seconds(&t_start, &t_stop);

    printf("TOTAL SOLUTIONS: %i \n", total);
    printf("WALL TIME: %ld microseconds\n", wall);

    for(int i = 0; i < N; i++){
        free(arg[i]);
    }
}