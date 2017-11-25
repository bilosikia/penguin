#pragma once

#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <iostream>
#include <wait.h>

void go() {
    const std::string path = "e:/a";
    if (mkfifo(path.c_str(), S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST) {
        perror("makfifo error");
        return;
    }
    switch (fork()) {
        case -1: {
            perror("fork error");
            return;
        }
        case 0: {
            int fd = open(path.c_str(), O_RDONLY);
            if(fd == -1) {
                perror("read open error");
                return;
            }
            unlink(path.c_str());
            char s[100];
            size_t pos = 0;
            while(true) {
                int len = read(fd, s + pos, 100 - pos) == -1;
                if(len == -1) {
                    perror("read error");
                    return;
                }
                else if (len == 0) {
                    break;
                }
                else {
                    pos += len;
                }
            }
            std::cout << s << std::endl;
            close(fd);
            exit(EXIT_SUCCESS);
        }
        default: {
            int fd = open(path.c_str(), O_WRONLY);
            if(fd == -1) {
                perror("write open error");
                return;
            }
            unlink(path.c_str());
            std::string s{"hello"};
            if(write(fd, s.c_str(), s.length()) == -1) {
                perror("write error");
                return;
            }
            close(fd);
            wait(nullptr);
        }
    }
}


