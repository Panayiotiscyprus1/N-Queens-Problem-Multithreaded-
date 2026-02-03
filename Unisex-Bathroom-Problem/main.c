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
    while (1){
        sem_wait(&e);
        if (nw>0){
            dm++;
            sem_post(&e);
            sem_wait(&m);
        }
        nm++;
        if (dm>0){
            dm--;
            sem_post(&m);
        } else {
            sem_post(&e);
        }

        // Critical Section

        sem_wait(&e);
        nm--;
        if (nm == 0 && dw > 0){
            dw--;
            sem_post(&w);
        } else {
            sem_post(&e);
        }
    }
}

void *Women(void *arg){
    while(1){
        sem_wait(&e);
        if(nm > 0){
            dw++;
            sem_post(&e);
            sem_wait(&w);
        }
        nw++;
        if(dw > 0){
            dw--;
            sem_post(&w);
        }else{
            sem_post(&e);
        }

        // do dis do dat, nootropia type shi

        sem_wait(&e);
        nw--;
        if(nw == 0 && dm > 0){
            dm--;
            sem_post(&m);
        }else{
            sem_post(&e);
        }
    }    
}

int main(){
    sem_init(&e, SHARED, 1);
    sem_init(&m, SHARED, 0);
    sem_init(&w, SHARED, 0);
}