#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#ifdef __APPLE__
    #include <dispatch/dispatch.h>
    typedef dispatch_semaphore_t sem_type;
    #define SEM_WAIT(s)   dispatch_semaphore_wait(s, DISPATCH_TIME_FOREVER)
    #define SEM_POST(s)   dispatch_semaphore_signal(s)
    #define SEM_INIT(s, v) s = dispatch_semaphore_create(v)
#else
    #include <semaphore.h>
    typedef sem_t sem_type;
    #define SEM_WAIT(s)   sem_wait(&s)
    #define SEM_POST(s)   sem_post(&s)
    #define SEM_INIT(s, v) sem_init(&s, 0, v)
#endif

#define SHARED 0
#define MEN 6
#define WOMEN 6
#define ITER 10

void *Men(void *);
void *Women(void *);

int nm, nw = 0;
int dm, dw = 0;

sem_t e, m, w, q;

void delay(int min, int max){
    int t = min + rand()% (max-min+1);
    sleep(t);
}

void *Men(void *arg){
    int id = *((int *) arg);
    for (int i = 0; i<ITER; i++){

        delay(5,10); // waiting to need to go to the bathroom again

        sem_wait(&q);
        sem_wait(&e);
        if (nw>0 || dw >0){
            dm++;
            sem_post(&e);
            sem_post(&q);
            sem_wait(&m);
        }else{
            sem_post(&q);
        }
        
        nm++;
        if (dm>0){
            dm--;
            sem_post(&m);
        } else {
            sem_post(&e);
        }

        printf("MAN   %2d  (inside: men=%d women=%d)\n", id, nm, nw);
        // fflush(stdout);
        delay(1,5); // do dis do dat, nootropia type shi
        

        sem_wait(&e);
        nm--;
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

        nw++;
        if(dw > 0){
            dw--;
            sem_post(&w);
        }else{
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
        }else{
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

}