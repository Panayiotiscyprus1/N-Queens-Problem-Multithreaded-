#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define SHARED 1

void *Men(void *);
void *Women(void *);

int nm, nw = 0;
int dm, dw = 0;

sem_t e, m, w;

void *Men(void *arg){

}

void *Women(void *arg){
    
}

int main(){
    sem_init(&e, SHARED, 1);
    sem_init(&m, SHARED, 0);
    sem_init(&w, SHARED, 0);
}