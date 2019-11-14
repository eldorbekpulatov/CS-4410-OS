#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../a2/rthread.h"

// https://piazza.com/class/jw5gjfzlolt5di?cid=211

/* This implementation of reader/writer lock uses split binary semaphores. */

/* Definition of shared state. */
struct rwlock {
    /* These three semaphores should always sum to 1. */
    rthread_sema_t mutex;
    rthread_sema_t readerSema;
    rthread_sema_t writerSema;

    /* Variables to keep track of the state. */
    int nReadersEntered, nReadersWaiting;
    int nWritersEntered, nWritersWaiting;
};

/* Initialize semaphores and state.*/
void rw_init(struct rwlock *rw){
    rthread_sema_init(&rw->mutex, 1);
    rthread_sema_init(&rw->readerSema, 0);
    rthread_sema_init(&rw->writerSema, 0);
    rw->nReadersEntered = rw->nReadersWaiting = 0;
    rw->nWritersEntered = rw->nWritersWaiting = 0;
}

/* Implementation of 'V hat' */
void rw_vacateOne(struct rwlock *rw){
    /* If there are no writers in the critical section and there are readers waiting,
     * release one of the waiting readers. */
    if (rw->nWritersEntered == 0 && rw->nReadersWaiting > 0) {
        rw->nReadersWaiting--;
        rthread_sema_vacate(&rw->readerSema);
    }

    /* If there's nobody in the critical section and there are writers waiting,
     * release one of the writers. */
    else if (rw->nReadersEntered == 0 && rw->nWritersEntered == 0 &&
                rw->nWritersWaiting > 0) {
        rw->nWritersWaiting--;
        rthread_sema_vacate(&rw->writerSema);
    }

    /* If neither is the case, just stop protecting the shared variables. */
    else {
        rthread_sema_vacate(&rw->mutex);
    }
}

void rw_enter_read(struct rwlock *rw){
    rthread_sema_procure(&rw->mutex);

    assert(rw->nWritersEntered == 0 || (rw->nWritersEntered == 1 && rw->nReadersEntered == 0));

    if (rw->nWritersEntered > 0) {
        rw->nReadersWaiting++;
        rw_vacateOne(rw);
        rthread_sema_procure(&rw->readerSema);
    }

    assert(rw->nWritersEntered == 0);
    rw->nReadersEntered++;
    rw_vacateOne(rw);
}

void rw_exit_read(struct rwlock *rw){
    rthread_sema_procure(&rw->mutex);
    assert(rw->nWritersEntered == 0);
    assert(rw->nReadersEntered > 0);
    rw->nReadersEntered--;
    rw_vacateOne(rw);
}

void rw_enter_write(struct rwlock *rw){
    rthread_sema_procure(&rw->mutex);

    assert(rw->nWritersEntered == 0 || (rw->nWritersEntered == 1 && rw->nReadersEntered == 0));

    if (rw->nReadersEntered > 0 || rw->nWritersEntered > 0) {
        rw->nWritersWaiting++;
        rw_vacateOne(rw);
        rthread_sema_procure(&rw->writerSema);
    }

    assert(rw->nReadersEntered == 0);
    assert(rw->nWritersEntered == 0);
    rw->nWritersEntered = 1;
    rw_vacateOne(rw);
}

void rw_exit_write(struct rwlock *rw){
    rthread_sema_procure(&rw->mutex);
    assert(rw->nReadersEntered == 0);
    assert(rw->nWritersEntered == 1);
    rw->nWritersEntered = 0;
    rw_vacateOne(rw);
}

#define NREADERS        4
#define NWRITERS        2
#define NEXPERIMENTS    3

char *readers[NREADERS] = { "r1", "r2", "r3", "r4" };
char *writers[NWRITERS] = { "w1", "w2" };

void reader(void *shared, void *arg){
    struct rwlock *rw = shared;
    char *name = arg;

    for (int i = 0; i < NEXPERIMENTS; i++) {
        printf("reader %s waiting for lock\n", name);
        rw_enter_read(rw);
        printf("reader %s has lock\n", name);
        rthread_delay(random() % 3000);
        printf("reader %s releases lock\n", name);
        rw_exit_read(rw);
        rthread_delay(random() % 3000);
    }
    printf("reader %s is done\n", name);
}

void writer(void *shared, void *arg){
    struct rwlock *rw = shared;
    char *name = arg;

    for (int i = 0; i < NEXPERIMENTS; i++) {
        printf("writer %s waiting for lock\n", name);
        rw_enter_write(rw);
        printf("writer %s has lock\n", name);
        rthread_delay(random() % 3000);
        printf("writer %s releases lock\n", name);
        rw_exit_write(rw);
        rthread_delay(random() % 3000);
    }
    printf("writer %s is done\n", name);
}

int main(){
    struct rwlock rw;

    rw_init(&rw);
    for (int i = 0; i < NREADERS; i++) {
        rthread_create(reader, &rw, readers[i]);
    }
    for (int i = 0; i < NWRITERS; i++) {
        rthread_create(writer, &rw, writers[i]);
    }
    rthread_run();
    return 0;
}
