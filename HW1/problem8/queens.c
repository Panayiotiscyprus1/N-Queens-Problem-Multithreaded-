#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "safe.h"

#define N 8

int results[N];
int total = 0;

typedef struct {
    int col;
    int row;
    int **board;
} solveArg;


void *solve(void *arg){

    solveArg *args = (solveArg *)arg; 
    int col = args->col;
    int row = args->row;
    int **board = args->board;
    int id = row;

    if(col == N){
        results[id]+=1;
        printf("I am thread %i with count %i \n", id, results[id]);
        printboard(board);
        return(NULL);
    }

    for(int r = row; r < N; r++){
        if (safe(r, col, board)){
            board[r][col] = 1;
            
            args->row = args->row + 1;
            args->board = board;

            solve(args);
            board[r][col] = 0;
        }
    }
    return(NULL);
}

int main(){
    solveArg **arg = malloc(N*sizeof(*arg));

    for(int i=0; i<N; i++){
        arg[i]->col=0;
        arg[i]->row=i;
        arg[i]->board=initboard();
    }


    pthread_t tid[N];
    for(long i=0; i<N; i++){ 
        pthread_create(&tid[i], NULL, (void *)solve, arg[i]); 
    }

    for(int i=0; i<N; i++){
        pthread_join(tid[i], NULL);
        total += results[i];
    }

    printf("%i", total);
}