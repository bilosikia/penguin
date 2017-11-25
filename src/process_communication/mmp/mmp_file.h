#pragma once

#include <string>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

void go() {
    std::string path = "e:/a";
    int fd = open(path.c_str(), O_RDWR);
    if(fd == -1) {
        perror("open file error");
        return;
    }
    struct stat st;
    if(fstat(fd, &st) == -1) {
        perror("fstat error");
        return;
    }
    void *addr = mmap(nullptr, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED) {
        perror("map error");
        return;
    }
    close(fd);
    write(STDOUT_FILENO, addr, st.st_size);
    memset(addr, 0, st.st_size);
    if(msync(addr, st.st_size, MS_SYNC) == -1) {
        perror("msync error");
        return;
    }
    munmap(addr, st.st_size);
}
