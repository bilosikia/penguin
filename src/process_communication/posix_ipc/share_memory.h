#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

void go() {
    int fd;
    if( (fd = shm_open("/shm", O_CREAT | O_RDWR, S_IRUSR | S_IWGRP)) == -1) {
        perror("open shm error");
        return;
    }
    shm_unlink("/shm");
    int size = 10;
    ftruncate(fd, size);
    void *addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == nullptr) {
        return;
    }

    switch(fork()) {
        case -1: {
            perror("fork error");
            close(fd);
            return;
        }
        case 0: {
            sleep(5);
            std::cout << (char *)addr << std::endl;
            break;
        }
        default: {
            memcpy(addr, "hello", sizeof("hello"));
            wait(nullptr);
            close(fd);
        }
    }
}