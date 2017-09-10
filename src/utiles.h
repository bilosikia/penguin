#pragma once

#include <unistd.h>
#include "err_handle.h"

class FdGuard {
private:
    int fd_;
public:
    FdGuard(int fd) {
        fd_ = fd;
    }

    int getFd() {
        return  fd_;
    }

    // 如果要检查是否关闭成功，调用此接口
    int closeFd() {
        return  close(fd_);
    }

    ~FdGuard() {
        if(fd_ != -1) {
            if(close(fd_) == -1) {
                printErr( std::string("close fd faild: ") + std::to_string(fd_));
            }
        }
    }

    // 不可拷贝
    FdGuard(const FdGuard &) = delete;
    // 不可赋值
    FdGuard &operator=(const FdGuard &) = delete;
};