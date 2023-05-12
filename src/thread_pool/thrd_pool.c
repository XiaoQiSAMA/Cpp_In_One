#include<pthread.h>
#include<stdint.h>
#include<stddef.h>
#include<stdlib.h>

#include "thrd_pool.h"

// 任务回调结构
typedef struct task_t
{
    handler_pt func;
    void *arg;
} task_t;

// 任务队列
typedef struct task_queue_t
{
    uint32_t head;
    uint32_t tail;
    uint32_t count;
    task_t *queue;
} task_queue_t;

// 线程池结构
struct thread_pool_t
{
    pthread_mutex_t mutex;  // 锁
    pthread_cond_t condition;   // 状态
    pthread_t *threads;
    task_queue_t task_queue;

    int closed;
    int started;

    int thrd_count;
    int queue_size;
};

static int thread_pool_free(thread_pool_t *pool);

static void * thread_worker(void *arg){
    thread_pool_t *pool = (thread_pool_t *)arg;
    task_queue_t *que;
    task_t task;

    // 线程任务调度
    for (;;){
        // 线程上锁
        pthread_mutex_lock(&(pool->mutex));
        que = &pool->task_queue;
        
        // """
        // 虚假唤醒
        // old linux pthread_cond_signal回唤醒多个线程，但是任务数不够分配，产生空指针
        // now 
        // 1. 可能被其他信号唤醒
        // 2. 业务场景造成
        // """

        // 线程调度
        // 当队列无任务 && 线程关闭 ==> 休眠
        while (que->count == 0 && pool->closed == 0){
            // 先释放 mutex锁
            // 阻塞在 condition（休眠）
            // ======= 唤醒 ========
            // 解除阻塞
            // 加上 mutex
            pthread_cond_wait(&(pool->condition), &(pool->mutex));
        }

        // 当前线程关闭，退出循环
        if (pool->closed == 1){
            break;
        }
        // 当前有任务
        // 从任务队列中取任务 
        task = que->queue[que->head];
        que->head = (que->head+1) % pool->queue_size;
        que->count--;
        pthread_mutex_unlock(&(pool->mutex));
        // 执行任务
        (*(task.func))(task.arg);
    }

    // 池的调度
    pool->started--;
    pthread_mutex_unlock(&(pool->mutex));
    pthread_exit(NULL);
    return NULL;
}

thread_pool_t *thread_pool_create(int thrd_count, int queue_size){
    thread_pool_t *pool;
    if (thrd_count <= 0 || queue_size <= 0)
        return NULL;
    
    pool = (thread_pool_t *)malloc(sizeof(*pool));
    if (pool == NULL)
        return NULL;
    
    // 初始化thread_pool_t中的参数
    pool->closed = pool->started = 0;
    // thrd_count设为0，防止后续申请任务时内存不足导致创建失败时，导致后续出错
    pool->thrd_count = 0;
    pool->queue_size = queue_size;

    // 初始化task_queue_t
    pool->task_queue.head = pool->task_queue.tail = pool->task_queue.count = 0;
    pool->task_queue.queue = (task_t *)malloc(sizeof(task_t) * queue_size);
    if (pool->task_queue.queue == NULL){
        // free(pool);
        thread_pool_free(pool);
        return NULL;
    }

    // 初始化thread_pool_t中的threads
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thrd_count);
    if (pool->threads == NULL){
        // free(pool->task_queue.queue);
        // free(pool);
        thread_pool_free(pool);
        return NULL;
    }
    
    // 创建线程
    for (int i = 0; i < thrd_count; i++){
        if (pthread_create(&(pool->threads[i]), NULL, thread_worker, (void *)pool) != 0){
            // 失败
            return NULL;
        }
        pool->started++;
        // 创建成功再增加thrd_count
        pool->thrd_count++;
    }

    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->condition, NULL);
    return pool;
        
}

// enum {
//     POOL_IS_NULL = -1;
//     POOL_LOCK_FAIL = -2;
// }

static int thread_pool_free(thread_pool_t *pool){
    // 按照创建pool的反方向来析构
    if (pool == NULL || pool->started <= 0){
        return -1;
    }

    if (pool->threads){
        free(pool->threads);
        pool->threads = NULL;

        pthread_mutex_lock(&(pool->mutex));
        pthread_mutex_destroy(&(pool->mutex));
        pthread_cond_destroy(&(pool->condition));
    }

    if (pool->task_queue.queue){
        free(pool->task_queue.queue);
        pool->task_queue.queue = NULL;
    }

    free(pool);
    return 0;
}

int thread_pool_destory(thread_pool_t * pool){
    if (pool == NULL){
        return -1;
    }
    // 避免其他线程往pool里加任务
    if (pthread_mutex_lock(&(pool->mutex)) != 0){
        return -2;
    }

    if (pool->closed == 1){
        return -1;
    }

    pool->closed = 1;

    // 唤醒所有线程
    if (pthread_cond_broadcast(&(pool->condition)) != 0 || pthread_mutex_unlock(&(pool->mutex)) != 0){
        return -2;
    }

    wait_all_done(pool);
    thread_pool_free(pool);
    return 0;

}

// 发布任务
int thread_pool_post(thread_pool_t * pool, handler_pt func, void *arg){
    if (pool == NULL || func == NULL){
        return -1;
    }

    if (pthread_mutex_lock(&(pool->mutex)) != 0){
        return -2;
    }

    if (pool->closed == 1){
        pthread_mutex_unlock(&(pool->mutex));
        return -3;
    }

    // 任务队列满了
    if (pool->queue_size == pool->task_queue.count){
        pthread_mutex_unlock(&(pool->mutex));
        return -4;
    }

    // 往队列尾部添加任务
    task_queue_t *task_queue = &(pool->task_queue);
    task_t *task = &(task_queue->queue[task_queue->tail]);
    task->func = func;
    task->arg = arg;
    task_queue->tail = (task_queue->tail + 1) % pool->queue_size;
    task_queue->count++;

    // 唤醒线程
    if (pthread_cond_signal(&(pool->condition)) != 0){
        pthread_mutex_unlock(&(pool->mutex));
        return -5;
    }
    pthread_mutex_unlock(&(pool->mutex));
    return 0;

}

// 等待所有运行中的线程完成当前任务并退出
int wait_all_done(thread_pool_t *pool){
    int i, ret = 0;
    for (i = 0; i < pool->thrd_count; i++){
        if (pthread_join(pool->threads[i], NULL) != 0){
            // 发生错误
            ret = 1;
        }
    }
    return ret;
}