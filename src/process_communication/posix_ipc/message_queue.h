#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sstream>
#include <iostream>

const std::string mq = "/mq5";

void hander(union sigval val);

void registNotify(mqd_t *md)
{
    struct sigevent e;
    e.sigev_notify = SIGEV_THREAD;
    e.sigev_value.sival_ptr = md;
    e._sigev_un._sigev_thread._function = hander;
    e._sigev_un._sigev_thread._attribute = nullptr;

    mq_attr attr;
    if (mq_getattr(*md, &attr) == -1)
    {
        perror("get attr error");
        exit(EXIT_FAILURE);
    }
    std::cout << "current msgs:" << attr.mq_curmsgs << std::endl;

    if (mq_notify(*md, &e) == -1)
    {
        perror("notify error");
        exit(EXIT_FAILURE);
    }
}

void hander(union sigval val)
{
    std::cout << "d";
    mqd_t *md = (mqd_t *)val.sival_ptr;
    mq_attr attr;
    if (mq_getattr(*md, &attr) == -1)
    {
        perror("get attr error");
        exit(EXIT_FAILURE);
    }
    std::cout << "size:" << attr.mq_msgsize << std::endl;
    // char *buffer = new char[attr.mq_msgsize];
    char *buffer = new char[10];
    memset(buffer, 0, 10);

    registNotify(md);
    while (mq_receive(*md, buffer, attr.mq_msgsize, nullptr) >= 0)
    {
        std::cout << "child: " << buffer << std::endl;
    }
    if (errno != EAGAIN)
    {
        exit(EXIT_FAILURE);
    }

    delete[] buffer;
    pthread_exit(nullptr);
}

void go()
{
    mqd_t md = mq_open(mq.c_str(), O_CREAT | O_NONBLOCK | O_RDWR, S_IWUSR | S_IRGRP | S_IWGRP, nullptr);
    if (md == -1)
    {
        perror("open mq error");
        return -1;
    }
    mq_unlink(mq.c_str());
    switch (fork())
    {
    case -1:
    {
        perror("fork error");
        return -1;
    }
    case 0:
    {
        std::cout << "child process" << std::endl;
        registNotify(&md);
        pause();
        mq_close(md);
        return 0;
    }
    default:
    {
        sleep(2); // only mq is empty, the notify will send
        for (int i = 0; i < 10; i++)
        {
            std::stringstream s;
            s << i;
            std::cout << s.str() << std::endl;
            if (mq_send(md, s.str().c_str(), s.str().length(), 0) == -1)
            {
                if (errno == EAGAIN)
                {
                    i--;
                    sleep(1000);
                    continue;
                }
                else
                {
                    perror("send error");
                    return -1;
                }
            }
        }
        wait(nullptr);
        mq_close(md);
    }
    }
    return 0;
}