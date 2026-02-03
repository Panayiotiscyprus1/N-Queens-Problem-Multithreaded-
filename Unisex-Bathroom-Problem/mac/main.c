#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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

#define MEN 6
#define WOMEN 6
#define ITER 5

int nm = 0, nw = 0; // Number inside
int dm = 0, dw = 0; // Number delayed (waiting)
sem_type e, m, w, q;

void delay(int min, int max) {
    sleep(min + rand() % (max - min + 1));
}

void *Men(void *arg) {
    int id = *((int *) arg);
    for (int i = 0; i < ITER; i++) {
        delay(5, 15);

        // ENTRY
        SEM_WAIT(q);
        SEM_WAIT(e);
        if (nw > 0 || dw > 0) { // If women are inside or waiting
            dm++;
            SEM_POST(e);
            SEM_POST(q);
            SEM_WAIT(m); 
        }else{
            SEM_POST(q);
        }

        nm++;
        if (dm > 0) {
            dm--;
            SEM_POST(m); // Pass baton to next waiting man
        } else {
            SEM_POST(e);
        }

        printf("MAN   %2d (inside: m=%d w=%d)\n", id, nm, nw);
        delay(1, 3);

        // EXIT
        SEM_WAIT(e);
        nm--;
        if (nm == 0 && dw > 0) {
            dw--;
            SEM_POST(w); // Room empty? Let a woman in
        } else {
            SEM_POST(e);
        }
    }
    return NULL;
}

void *Women(void *arg) {
    int id = *((int *) arg);
    for (int i = 0; i < ITER; i++) {
        delay(5, 15);

        SEM_WAIT(q);
        SEM_WAIT(e);
        if (nm > 0 || dm > 0) {
            dw++;
            SEM_POST(e);
            SEM_POST(q);
            SEM_WAIT(w);
        }else{
            SEM_POST(q);
        }

        nw++;
        if (dw > 0) {
            dw--;
            SEM_POST(w);
        } else {
            SEM_POST(e);
        }

        printf("WOMAN %2d (inside: m=%d w=%d)\n", id, nm, nw);
        delay(1, 3);

        SEM_WAIT(e);
        nw--;
        if (nw == 0 && dm > 0) {
            dm--;
            SEM_POST(m);
        } else {
            SEM_POST(e);
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    SEM_INIT(e, 1); SEM_INIT(q, 1);
    SEM_INIT(m, 0); SEM_INIT(w, 0);

    pthread_t men[MEN], women[WOMEN];
    int m_ids[MEN], w_ids[WOMEN];

    for (int i = 0; i < MEN; i++) { m_ids[i] = i; pthread_create(&men[i], NULL, Men, &m_ids[i]); }
    for (int i = 0; i < WOMEN; i++) { w_ids[i] = i; pthread_create(&women[i], NULL, Women, &w_ids[i]); }
    for (int i = 0; i < MEN; i++) pthread_join(men[i], NULL);
    for (int i = 0; i < WOMEN; i++) pthread_join(women[i], NULL);

    return 0;
}