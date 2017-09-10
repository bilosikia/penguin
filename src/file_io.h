#pragma once

#include <stdio.h>
#include <fcntl.h>
#include <sys/uio.h>
#include "utiles.h"

// 系统调用都是原子的
void go() {
    // fcntrl 获取访问模式，状态标志
    // 修改状态标志
    int flags = O_RDONLY | O_SYNC;
    FdGuard fd(open("./test", flags));
    if (fd.getFd() == -1) {
        printErr(nullptr);
    } else {
        int flg = fcntl(fd.getFd(), F_GETFL);
        if (flg == -1) {
            printErr(nullptr);
        } else {
            // 状态标记
            std::cout << static_cast<bool >( flags & flg) << std::endl;
            // 访问模式
            std::cout << ((flg & O_ACCMODE) == O_RDONLY) << std::endl;
            std::cout << ((flg & O_ACCMODE) == O_WRONLY) << std::endl;
            // 设置状态标记
            flg |= O_APPEND;
            if (fcntl(fd.getFd(), F_SETFL, flg) == -1) {
                printErr(nullptr);
            } else {
                std::cout << (flg & O_APPEND) << std::endl;
            }
        }
    }

    // 复制文件描述符
    int newFd = dup(fd.getFd());
    std::cout << newFd << std::endl;
    int newFd2 = dup2(fd.getFd(), 12);
    std::cout << newFd2 << std::endl;
    int newFd3 = fcntl(fd.getFd(), F_DUPFD, 13);
    std::cout << newFd3 << std::endl;
    int newFd4 = fcntl(fd.getFd(), F_DUPFD_CLOEXEC, 14);
    std::cout << newFd4 << std::endl;
    close(newFd);
    close(newFd3);
    close(newFd2);

    // 偏移量的文件操作 原子性，一次系统调用
    char buff[100] = {0};
    size_t readSize = pread(fd.getFd(), buff, 100, 10);
    if (readSize == -1) {
        perror(nullptr);
    } else {
        std::cout << buff << std::endl;
    }

    // 分散读取
    char buff1[50] = {0}, buff2[50] = {0}, buff3[50] = {0};
    struct iovec iovec1 = {
            iov_base: buff1,
            iov_len: 100
    };
    struct iovec iovec2 = {
            iov_base: buff2,
            iov_len: 100
    };
    struct iovec iovec3 = {
            iov_base: buff3,
            iov_len: 100
    };
    struct iovec iovecs[] = {iovec1, iovec2, iovec3};
    readSize = readv(fd.getFd(), iovecs, 3);
    if (readSize == -1) {
        printErr(nullptr);
    } else {
        std::cout << buff1 << buff2 << buff3 << std::endl;
    }

    // 在指定的偏移量处集中读取和写
    // preadv, pwritev();

    // 截断文件
    if (-1 == truncate("./test", 20)) {
        printErr(nullptr);
    }
//    if(ftruncate(fd.getFd(), 1000)) {
//        printErr(nullptr);
//    }

    // 创建临时文件
    char tempPath[] = "./tempXXXXXX";
    int tempFd = mkstemp(tempPath);
    std::cout << "temp path: " << tempFd << std::endl;
    unlink(tempPath);
    // 内部立即调用unlink
    FILE* tempFilePtr = tmpfile();
}