#include <stdio.h>
#include <stdlib.h>
#include "rthread.h"
struct shared_data
{
    int counter;
    rthread_lock_t lock;
};
void incrementer(void *shared, void *arg)
{
    struct shared_data *sd = shared;
    int *delta = arg;
    for (int i = 0; i < 1000000; i++)
        rthread_with(&sd->lock)
            sd->counter += *delta;
}
int main()
{
    struct shared_data sd;
    int up = 1, down = -1;
    sd.counter = 0;
    rthread_lock_init(&sd.lock);
    rthread_create(incrementer, &sd, &up);
    rthread_create(incrementer, &sd, &down);
    rthread_run();
    printf("counter = %d\n", sd.counter);
    return 0;
} 
