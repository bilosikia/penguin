#pragma once

#include <unistd.h>
#include <cstdio>
#include <string>
#include <sys/wait.h>
#include <iostream>

void go() {
    int fd[2];
    if (pipe2(fd) == -1) {
        perror("create pipe failed");
        return;
    }
    switch (fork()) {
        case -1: {
            perror("fork error");
            return;
        }
        case 0: {
            if (close(fd[1]) == -1) { // close write
                perror("close pipe write point error");
            }
            char s[100]= {0};
            size_t pos = 0;
            while(true) {
                int len = read(fd[0], s + pos, 100 - pos);
                if(len == -1) {
                    perror("read error");
                }
                else if (len == 0) {
                    break;
                }
                else {
                    pos += len;
                }
            }
            std::cout << s << std::endl;
            if(close(fd[0]) == -1) {
                perror("close read point error");
            }
            exit(EXIT_SUCCESS);
        }
        default: {
            if (close(fd[0]) == -1) {
                perror("close read point error");
            }
            std::string s{"hello"};
            if (write(fd[1], s.c_str(), s.length()) == -1) {
                perror("write error");
            }
            if(close(fd[1]) == -1) {
                perror("close pipe write point error");
            }
            wait(nullptr);
        }
    }
}
