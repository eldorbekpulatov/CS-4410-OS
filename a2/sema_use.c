#include <stdio.h>
#include <stdlib.h>
#include "rthread.h"
struct shared_data
{
    int counter;
    rthread_sema_t mutex;
};
void incrementer(void *shared, void *arg)
{
    struct shared_data *sd = shared;
    int *delta = arg;
    for (int i = 0; i < 1000000; i++)
    {
        rthread_sema_procure(&sd->mutex);
        sd->counter += *delta;
        rthread_sema_vacate(&sd->mutex);
    }
}
int main()
{
    struct shared_data sd;
    int up = 1, down = -1;
    sd.counter = 0;
    rthread_sema_init(&sd.mutex, 1);
    rthread_create(incrementer, &sd, &up);
    rthread_create(incrementer, &sd, &down);
    rthread_run();
    printf("counter = %d\n", sd.counter);
    return 0;
} 
