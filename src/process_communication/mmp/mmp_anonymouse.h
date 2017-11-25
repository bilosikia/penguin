#pragma once

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sys/wait.h>

void go() {
    const int len = 100;
//    int fd = open("/dev/zero", O_RDWR);
//    void *addr = mmap(nullptr, len, PROT_WRITE | PROT_READ, MAP_SHARED, -1, 0);
    void *addr = mmap(nullptr, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    switch (fork()) {
        case -1: {
            perror("fork error");
            return;
        }
        case 0: {
            *(int *)addr = 10;
            std::cout << *(int *)(addr) << std::endl;
            exit(EXIT_SUCCESS);
        }
        default: {
            wait(nullptr);
            std::cout << *(int *)(addr) << std::endl;
            munmap(addr, len);
//            close(fd);
        }
    }
}