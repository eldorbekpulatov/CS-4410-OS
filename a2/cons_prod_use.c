#include <stdio.h>
#include <stdlib.h>
#include "rthread.h"

#define BB_QSIZE 10 // size of bounded buffer
// Bounded buffer code goes here

struct bounded_buffer;
void bb_init(struct bounded_buffer *bb);
void bb_produce(struct bounded_buffer *bb, int item);
int bb_consume(struct bounded_buffer *bb);
struct bounded_buffer
{
    int queue[BB_QSIZE];      // the item storage
    int in;                   // where to insert a new item
    int out;                  // where to retrieve an item
    rthread_sema_t in_mutex;  // mutex on ->in
    rthread_sema_t out_mutex; // mutex on ->out
    rthread_sema_t n_empty;   // counts #empty slots
    rthread_sema_t n_full;    // counts #full slots
};
void bb_init(struct bounded_buffer *bb)
{
    bb->in = bb->out = 0;
    rthread_sema_init(&bb->in_mutex, 1);
    rthread_sema_init(&bb->out_mutex, 1);
    rthread_sema_init(&bb->n_empty, BB_QSIZE);
    rthread_sema_init(&bb->n_full, 0);
}
void bb_produce(struct bounded_buffer *bb, int item)
{
    rthread_sema_procure(&bb->n_empty);
    rthread_sema_procure(&bb->in_mutex);
    bb->queue[bb->in] = item;
    bb->in = (bb->in + 1) % BB_QSIZE;
    rthread_sema_vacate(&bb->in_mutex);
    rthread_sema_vacate(&bb->n_full);
}
int bb_consume(struct bounded_buffer *bb)
{
    int item;
    rthread_sema_procure(&bb->n_full);
    rthread_sema_procure(&bb->out_mutex);
    item = bb->queue[bb->out];
    bb->out = (bb->out + 1) % BB_QSIZE;
    rthread_sema_vacate(&bb->out_mutex);
    rthread_sema_vacate(&bb->n_empty);
    return item;
}

#define NPRODUCERS 4
#define NCONSUMERS 2
#define SCALE 500000

void producer(void *shared, void *arg)
{
    for (int i = 0; i < SCALE * NCONSUMERS; i++)
        bb_produce(shared, i);
}
void consumer(void *shared, void *arg)
{
    for (int i = 0; i < SCALE * NPRODUCERS; i++)
        (void)bb_consume(shared);
}
int main()
{
    struct bounded_buffer bb;
    bb_init(&bb);
    for (int i = 0; i < NPRODUCERS; i++)
        rthread_create(producer, &bb, 0);
    for (int i = 0; i < NCONSUMERS; i++)
        rthread_create(consumer, &bb, 0);
    rthread_run();
    return 0;
} 
