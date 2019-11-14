#include <stdio.h>
#include <stdlib.h>
#include "rthread.h"
void incrementer(void *shared, void *arg)
{
    int *counter = shared;
    int *delta = arg;
    for (int i = 0; i < 1000000; i++)
        *counter += *delta;
}
int main()
{
    int counter = 0;       // shared among threads
    int up = 1, down = -1; // arguments to threads
    rthread_create(incrementer, &counter, &up);
    rthread_create(incrementer, &counter, &down);
    rthread_run();
    printf("counter = %d\n", counter);
    return 0;
} 
