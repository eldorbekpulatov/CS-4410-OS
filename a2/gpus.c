#include <stdio.h>
#include <stdlib.h>
#include "rthread.h"
rthread_lock_t print_lock;
// YOUR CODE GOES HERE
#define NGPUS 10
#include <string.h>
#include <assert.h>
struct gpu_info
{
};

void gi_init(struct gpu_info *gi);
void gi_alloc(struct gpu_info *gi,
              unsigned int ngpus,
              /* OUT */ unsigned int gpus[]);
void gi_release(struct gpu_info *gi,
                unsigned int ngpus,
                /* IN */ unsigned int gpus[]);
void gi_free(struct gpu_info *gi);

void gi_init(struct gpu_info *gi) {
}

void gi_alloc(struct gpu_info *gi,
              unsigned int ngus,
	      unsigned int gpus[]) {
}

void gi_release(struct gpu_info *gi,
                unsigned int ngpus,
                /* IN */ unsigned int gpus[]) {
}

void gi_free(struct gpu_info *gi) {
}



// =============================================================
//                  Test Code for GPU Allocator
// =============================================================
void gpu_user(void *shared, void *arg)
{
    struct gpu_info *gi = shared;
    unsigned int gpus[NGPUS];
    for (int i = 0; i < 5; i++)
    {
        rthread_delay(random() % 3000);
        unsigned int n = 1 + (random() % NGPUS);
        rthread_with(&print_lock)
        printf("%s %d wants %u gpus\n", (char *) arg, i, n);
        gi_alloc(gi, n, gpus);
        rthread_with(&print_lock)
        {
            printf("%s %d allocated:", (char *) arg, i);
            for (int i = 0; i < n; i++)
                printf(" %d", gpus[i]);
            printf("\n");
        }
        rthread_delay(random() % 3000);
        rthread_with(&print_lock)
            printf("%s %d releases gpus\n", (char *) arg, i);
        gi_release(gi, n, gpus);
    }
}
int main()
{
    rthread_lock_init(&print_lock);
    struct gpu_info gi;
    gi_init(&gi);
    rthread_create(gpu_user, &gi, "Jane");
    rthread_create(gpu_user, &gi, "Joe");
    rthread_run();
    gi_free(&gi);
    return 0;
}
