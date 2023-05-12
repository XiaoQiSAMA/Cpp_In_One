#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

typedef struct thread_pool_t thread_pool_t;
typedef void (*handler_pt)(void *); 

thread_pool_t *thread_pool_create(int thrd_count, int queue_size);

int thread_pool_destory(thread_pool_t * pool);

int thread_pool_post(thread_pool_t * pool, handler_pt func, void *args);

int wait_all_done(thread_pool_t *pool);

#endif