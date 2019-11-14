#include "rthread.h"

#define MIDDLE 0
#define HIGH 1
#define NLANES 7

struct pool
{
    rthread_lock_t lock;
    int count;
    int numHigh;
    int waitHigh;
    int numMiddle;
    int waitMiddle;
    int turn;
    rthread_cv_t qHigh;
    rthread_cv_t qMiddle;
};

void pool_init(struct pool *pool)
{
    memset(pool, 0, sizeof(*pool));
    pool->count = 0;
    pool->numHigh = 0;
    pool->numMiddle = 0;
    pool->waitHigh = 0;
    pool->waitMiddle = 0;
    pool->turn = 2;
    rthread_lock_init(&pool->lock);
    rthread_cv_init(&pool->qHigh, &pool->lock);
    rthread_cv_init(&pool->qMiddle, &pool->lock);
}

void pool_enter(struct pool *pool, int level)
{
    rthread_with(&pool->lock)
    {   
        if (level == MIDDLE){
            pool->waitMiddle += 1;
            while (pool->numHigh > 0 | pool->count == NLANES | (pool->turn == HIGH && pool->numMiddle > 0)){
                if (pool->numHigh > 0 ) { pool->turn = MIDDLE; }
                rthread_cv_wait(&pool->qMiddle); 
            }
            pool->waitMiddle -= 1;
            pool->count += 1;
            pool->numMiddle +=1;
        }
        if (level == HIGH){
            pool->waitHigh += 1;
            while (pool->numMiddle > 0 | pool->count == NLANES | (pool->turn == MIDDLE && pool->numHigh > 0)){
                if (pool->numMiddle > 0  ) { pool->turn = HIGH; }
                rthread_cv_wait(&pool->qHigh); 
            }
            pool->waitHigh -= 1;
            pool->count += 1;
            pool->numHigh +=1;  
        }
        
    }
}

void pool_exit(struct pool *pool, int level)
{
    rthread_with(&pool->lock)
    {   
        if (level == MIDDLE){
            pool->count -= 1;
            pool->numMiddle -=1;

            if ( pool->waitHigh > 0 && pool->numMiddle == 0){
                pool->turn = HIGH;
                rthread_cv_notifyAll(&pool->qHigh);
            }else{
                rthread_cv_notify(&pool->qMiddle);
            }
        }

        if (level == HIGH){
            pool->count -= 1;
            pool->numHigh -=1;

            if ( pool->waitMiddle > 0 && pool->numHigh == 0){
                pool->turn = MIDDLE;
                rthread_cv_notifyAll(&pool->qMiddle);
            }else{
                rthread_cv_notify(&pool->qHigh);
            }
            
        }
    
    }
}