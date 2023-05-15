# Epoll + 多线程

epoll单线程代码参考虚拟机

## 伪代码

### 多个线程共同管理一个epoll fd，会导致对fd的重复读写。并且epoll在管理fd时，会由锁机制，影响性能

``` c++
void *func(void *arg){
    # int sockfd = *(int *)arg;
    int epfd = epoll_create();

    while (1){
        int clientfd = epoll_wait();
        epoll_ctl(epfd, clientfd);
    }
}

int main(){
    int epfd = epoll_create();
    int sockfd = init_server();

    epoll_CTL(epfd, sockfd);

    for (int i = 0; i < 10; i++){
        pthread_create(, func, epfd);
    }
}
```

### 子线程依次管理一个epoll fd（惊群问题）

``` c++
// 给sockfd上锁来解决惊群问题
lock_t lock;

void *func(void *arg){
    int sockfd = *(int *)arg;
    int epfd = epoll_create();

    while (1){
        if (lock == off){
            epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd);

            lock = on;

            int nready = epoll_wait();

        if (events[i].data.fd == listenfd){
            int clientfd = accept(listenfd, );
            epoll_ctl(epfd, clientfd);
        }
        } else {
            // 直接处理
        }
    }
}

int main(){
    int epfd = epoll_create();
    int sockfd = init_server();

    epoll_CTL(epfd, sockfd);

    for (int i = 0; i < 10; i++){
        pthread_create(, func, epfd);
    }
}
```

### 1+N模式。一个管理接入事件，N个对接入的事件进行事务处理

``` c++
int epfd[10];

void *func(void *arg){
    int sockfd = *(int *)arg;
    pthread_t selfid = pthread_self();

    // int epfd = epoll_create();
    epfd[selfid % 10] = epoll_create();

    while (1){
        if (lock == off){
            epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd);

            lock = on;

            int nready = epoll_wait();

        if (events[i].data.fd == listenfd){
            int clientfd = accept(listenfd, );
            epoll_ctl(epfd, clientfd);
        }
        } else {
            // 直接处理
        }
    }
}

int main(){
    int sockfd = init_server();

    // N: N个线程来分别管理epoll fd事件
    for (int i = 0; i < 10; i++){
        pthread_create(, func, epfd);
    }

    // 1: sockfd的监听
    while (1){
        poll();  // select/poll/epoll
        
        int clientfd = accept(sockfd);

        idx ++;
        idx %= 10;

        // 粗暴的负载均衡
        epoll_ctl(epfd[idx], EPOLL_CTL_ADD, clientfd, &ev);
    }
}
```

### M+N模式。M个管理接入事件，N个对接入的事件进行事务处理
