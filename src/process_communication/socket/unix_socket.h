#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#include <string>
#include <iostream>

void go()
{
    char socketPath[] = "/tmp/gkl_socket111";
    remove(socketPath);
    sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath, sizeof(socketPath));

    switch (fork())
    {
    case -1:
    {
        perror("fork error");
        return;
    }
    case 0:
    {
        sleep(1);
        int fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd == -1)
        {
            perror("socket create error");
            return;
        }

        if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        {
            std::cout << addr.sun_path << std::endl;
            perror("connect error");
            return;
        }
        std::string s;
        while (std::cin >> s)
        {
            if (send(fd, s.c_str(), s.length(), 0) == -1)
            {
                perror("send error");
                return;
            }
        }
        close(fd);
        return;
    }
    default:
    {
        int fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd == -1)
        {
            perror("create socket errir");
            return;
        }
        if(bind(fd, (sockaddr *)&addr, sizeof(addr)) == -1) {
            perror("bind error");
            return;
        }
        if(listen(fd, SOMAXCONN) == -1) {
            perror("listen error");
            return;
        }
        int clientFd = 0;
        while( (clientFd =accept(fd, nullptr, 0)) != -1) {
            char buffer[100] = {0};
            while(read(clientFd, buffer, 100) != -1) {
                std::cout << buffer;
                std::flush(std::cout);
                memset(buffer, 0, 100);
            }
            std::cout << std::endl;
            close(clientFd);
        }
        close(fd);
        unlink(socketPath);
        return;
    }

    }
}