#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

void go() {
    sockaddr_in sAddr;
    memset(&sAddr, 0 ,sizeof(sAddr));
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(6666);
    if(inet_pton(AF_INET, "127.0.0.1", &sAddr.sin_addr) == -1) {
        perror("pton error");
        return;
    }

    switch(fork()) {
        case -1: {
            perror("fork error");
            return;
        }
        case 0: {
            sleep(1);
            int fd = socket(AF_INET, SOCK_STREAM, 0);
            if(fd == -1) {
                perror("socket error");
                return;
            }
            if(connect(fd, (sockaddr *)&sAddr, sizeof(sAddr)) == -1) {
                perror("connect error");
                return;
            }
            char context[] = "你哈哦啊";
            if(send(fd, context, sizeof(context), 0) == -1) {
                perror("send error");
                return;
            }
            close(fd);
            return;
        }
        default: {
            int fd = socket(AF_INET, SOCK_STREAM, 0);
            if(fd == -1) {
                perror("socket error");
                return;
            }
            if(bind(fd, (sockaddr *)&sAddr, sizeof(sAddr)) == -1) {
                perror("bind error");
                return;
            }
            if(listen(fd, SOMAXCONN) == -1) {
                perror("listen error");
                return;
            }
            int clientFd = 0;
            sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            memset(&clientAddr, 0, sizeof(clientAddr));
            while((clientFd = accept(fd, (sockaddr *)&clientAddr, &clientAddrLen)) != -1) {
                char buffer[100] = {0};
                while(read(clientFd, buffer, 100) > 0) {
                    std::cout << buffer;
                    std::flush(std::cout);
                }
                close(clientFd);
            }
            close(fd);
            return;
        }
    }


}