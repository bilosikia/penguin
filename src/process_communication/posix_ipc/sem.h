#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int v = 0;
void* f(void *args) {
    sem_t *sem = (sem_t *)args;
    for(size_t i = 0; i < 1000000; i++) {
        if(sem_wait(sem) == -1) {
            perror("wait error");
            return nullptr;
        }
        v++;
        if(sem_post(sem) == -1) {
            perror("post error");
            return nullptr;
        }
    }
}

void go() {
    sem_t sem;
    if(sem_init(&sem, 0, 1) == -1) {
        perror("init semaphore error");
        return;
    }
    pthread_t t;
    if(pthread_create(&t, nullptr, f, &sem) == -1) {
        perror("thread create error");
        return;
    }

    for(size_t i = 0; i < 1000000; i++) {
        if(sem_wait(&sem) == -1) {
            perror("wait error");
            return;
        }
        v++;
        if(sem_post(&sem) == -1) {
            perror("post error");
            return;
        }
    }
   if(pthread_join(t,nullptr) != 0) {
       perror("join error");
       exit(EXIT_FAILURE);
   }
   sem_destroy(&sem);
   std::cout << v << std::endl;
}