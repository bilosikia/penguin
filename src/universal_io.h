#pragma once

#include <sys/fcntl.h>
#include "err_handle.h"
#include "utiles.h"

void go() {
    // 打开文件描述符, 最小可用
    int flags = O_RDONLY | O_SYNC;
    FdGuard fd(open("../main.cpp", flags));
    if (fd.getFd() == -1) {
        printErr(nullptr);
    } else {
        std::cout << "open success" << std::endl;
        char buffer[100] = {0};
        size_t readSize = read(fd.getFd(), buffer, 100);
        if (readSize == -1) {
            printErr(nullptr);
        }
        std::cout << buffer << std::endl;
    }

    // 打开目录
    flags = O_RDONLY | O_DIRECTORY;
    FdGuard dirFd(open("..", flags));
    if (dirFd.getFd() == -1) {
        printErr(nullptr);
    } else {
        std::cout << "open directory success" << std::endl;
    }

    // 创建文件
    flags = O_CREAT | O_WRONLY | O_EXCL;
    mode_t mode = 777;
    FdGuard createdFd(open("./test", flags, mode));
    if (createdFd.getFd() == -1) {
        printErr(nullptr);
    } else {
        size_t start = lseek(fd.getFd(), 0, SEEK_SET);
        if (start == -1) {
            printErr(nullptr);
        }
        char buffer[100] = {0};
        size_t readSize = read(fd.getFd(), buffer, 100);
        if (readSize == -1) {
            printErr(nullptr);
        }
        size_t writedSize = write(createdFd.getFd(), buffer, readSize);
        if (writedSize == -1) {
            printErr(nullptr);
        }
        std::cout << "create file success" << std::endl;
    }

    // 文件空洞
    size_t currOffet = lseek(createdFd.getFd(), 100, SEEK_END);
    if (currOffet == -1) {
        printErr(nullptr);
    } else {
        if (write(createdFd.getFd(), "hello world", 100) == -1) {
            printErr(nullptr);
        }
    }
}