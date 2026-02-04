#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define SHARED 0
#define MEN 6
#define WOMEN 6
#define ITER 10

void *Men(void *);
void *Women(void *);

/*
 nm, nw : number of men / women currently inside the bathroom
 dm, dw : number of men / women delayed (waiting to enter)
*/
int nm = 0, nw = 0;
int dm = 0, dw = 0;

/*
 e : mutex semaphore protecting shared variables
 m : semaphore on which waiting men sleep
 w : semaphore on which waiting women sleep
 q : fairness / turnstile semaphore (prevents barging)
*/
sem_t e, m, w, q;

void delay(int min, int max){
    int t = min + rand() % (max - min + 1);
    sleep(t);
}

void *Men(void *arg){
    int id = *((int *) arg);
    for (int i = 0; i < ITER; i++){

        delay(5,10); // simulate time before needing the bathroom


        sem_wait(&q);   // enter fairness queue
        sem_wait(&e);   // lock shared state to increase dm

        // If women are inside or women are waiting, men must wait
        if (nw > 0 || dw > 0){
            dm++;
            sem_post(&e);

            // leave fairness queue
            sem_post(&q);
            
            // wait until awakened
            sem_wait(&m);
        } else {
            // no conflict, leave fairness queue
            sem_post(&q);
        }

        nm++;   // man enters bathroom

        // Wake another waiting man if any, otherwise release mutex
        if (dm > 0){
            dm--;
            sem_post(&m);
        } else {
            sem_post(&e);
        }

        printf("MAN   %2d  (inside: men=%d women=%d)\n", id, nm, nw);

        delay(1,5); // simulate bathroom usage

        sem_wait(&e);   // lock shared state to decrement nm
        nm--;

        // If bathroom becomes empty and women are waiting, wake one woman
        if (nm == 0 && dw > 0){
            dw--;
            sem_post(&w);
        } else {
            sem_post(&e);
        }
    }
    return NULL;
}

void *Women(void *arg){
    for (int i = 0; i<ITER; i++){

        int id = *((int *) arg);
        delay(5,10); // waiting to need to go to the bathroom again

        sem_wait(&q);
        sem_wait(&e);
        if(nm > 0 || dm>0){
            dw++;
            sem_post(&e);
            sem_post(&q);
            sem_wait(&w);
        }else{
            sem_post(&q);
        }

        nw++;   // woman enters bathroom

        // Wake another waiting woman if any, otherwise release mutex
        if (dw > 0){
            dw--;
            sem_post(&w);
        } else {
            sem_post(&e);
        }

        printf("WOMAN %2d  (inside: men=%d women=%d)\n", id, nm, nw);
        // fflush(stdout);
        delay(1,5); // do dis do dat, read a book (mega alexantro)

        sem_wait(&e);
        nw--;
        if(nw == 0 && dm > 0){
            dm--;
            sem_post(&m);
        } else {
            sem_post(&e);
        }
    }
    return NULL;
}

int main(){
    srand(time(NULL));
    sem_init(&e, SHARED, 1);
    sem_init(&q, SHARED, 1);
    sem_init(&m, SHARED, 0);
    sem_init(&w, SHARED, 0);

    pthread_t men[MEN], women[WOMEN];
    int men_ids[MEN], women_ids[WOMEN];

    for (int i = 0; i<MEN; i++){
        men_ids[i] = i;
        pthread_create(&men[i], NULL, Men, &men_ids[i]);
    }

    for (int i = 0; i<WOMEN; i++){
        women_ids[i] = i;
        pthread_create(&women[i], NULL, Women, &women_ids[i]);
    }

    
    for (int i = 0; i<MEN; i++){ pthread_join(men[i], NULL); }
 
    for (int i = 0; i<WOMEN; i++){ pthread_join(women[i], NULL); }

    sem_destroy(&e);
    sem_destroy(&m);
    sem_destroy(&w);
    sem_destroy(&q);

    return 0;
}